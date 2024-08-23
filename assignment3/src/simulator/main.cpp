#include "utils.h"
#include "AlgorithmRegistration.h"
#include "Simulator.h"

std::string houseDirPath = "";
std::string algoDirPath = "";
int numOfThreads = 10;
bool summaryOnly = false;

void writeError(const std::string &fileName, const std::string &errorMessage) {
    std::ofstream errorFile(fileName);
    if (errorFile.is_open()) {
        errorFile << errorMessage << std::endl;
        errorFile.close();
    } else {
        std::cerr << "Failed to open error file: " << fileName << std::endl;
    }
}

void handleCommandLineArguments(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        std::istringstream ss(argv[i]);
        std::string key;
        std::string value;

        // Extract key and value
        if (std::getline(ss, key, '=') && std::getline(ss, value)) {
            key.erase(key.find_last_not_of(" \t\n\r\f\v") + 1); // Trim trailing spaces
            value.erase(0, value.find_first_not_of(" \t\n\r\f\v")); // Trim leading spaces

            if (key == "-house_path") {
                houseDirPath = value;
            } else if (key == "-algo_path") {
                algoDirPath = value;
            } else if (key == "-num_threads") {
                numOfThreads = std::stoi(value);
            } else if (key == "-summary_only") {
                summaryOnly = true;
            } else {
                throw std::runtime_error("Invalid argument key: " + key);
            }
        } else {
            throw std::runtime_error("Invalid argument format: " + std::string(argv[i]));
        }
    }
    // if no houseDirPath is given, search in local working directory
    if (houseDirPath.empty()) {
        houseDirPath = std::filesystem::current_path().string();
    }
    // if no algoDirPath is given, search in local working directory
    if (algoDirPath.empty()) {
        algoDirPath = std::filesystem::current_path().string();
    }
}

void runWrapper(std::pair<std::string, std::unique_ptr<AbstractAlgorithm>> houseAlgoPair) {
    const std::string &houseFilePath = houseAlgoPair.first;
    std::unique_ptr<AbstractAlgorithm> algo = std::move(houseAlgoPair.second);

    Simulator simulator;
    simulator.readHouseFile(houseFilePath);
    simulator.setAlgorithm(std::move(algo));
    simulator.run();
}


void worker(std::queue<std::pair<std::string, std::unique_ptr<AbstractAlgorithm>>> &tasks, std::mutex &queueMutex) {
    while (true) {
        std::pair<std::string, std::unique_ptr<AbstractAlgorithm>> task;

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (tasks.empty()) {
                return; // No more tasks, exit thread
            }
            // Take the next task
            task = std::move(tasks.front());
            tasks.pop();
        }

        runWrapper(std::move(task));
    }
}

int main(int argc, char **argv) {
    try {
        if (argc > 5) {
            throw std::runtime_error("up to 5 command-line arguments allowed");
        }
        handleCommandLineArguments(argc, argv);

        std::vector<std::string> houseFiles;
        std::vector<void *> algoHandles;

        // Iterate over houseDirPath to find .house files
        for (const auto &entry : std::filesystem::directory_iterator(houseDirPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".house") {
                // Try to open the .house file
                std::ifstream currHouseFile(entry.path());
                if (!currHouseFile.is_open()) {
                    std::string errorFileName = entry.path().stem().string() + ".error";
                    writeError(errorFileName, "Failed to open house file: " + entry.path().string());
                    continue; // Skip this file
                }

                // If the file opened successfully, add it to the houseFiles vector
                houseFiles.push_back(entry.path().string());

                // Close the file
                currHouseFile.close();
            }
        }

        // Iterate over algoDirPath to find .so files and open them with dlopen
        for (const auto &entry : std::filesystem::directory_iterator(algoDirPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".so") {
                void *handle = dlopen(entry.path().c_str(), RTLD_LAZY);
                if (!handle) {
                    std::string errorFileName = entry.path().stem().string() + ".error";
                    writeError(errorFileName,
                               "Failed to open algorithm file: " + entry.path().string() + "\ndlerror: " + dlerror());
                    continue; // Skip this file
                }

                // Use dlsym to get the registerAlgorithm function from the shared library
                auto registerAlgorithm = (void (*)()) dlsym(handle, "registerAlgorithm");
                if (!registerAlgorithm) {
                    std::string errorFileName = entry.path().stem().string() + ".error";
                    writeError(errorFileName, "Failed to find registerAlgorithm function: " + std::string(dlerror()));
                    dlclose(handle);
                    continue; // Skip this file
                }
                // Call the function to register the algorithm
                registerAlgorithm();

                // Store the handle for later dlclose
                algoHandles.push_back(handle);
            }
        }
        // Vector to store pairs of house files and algorithm handles
        std::vector<std::pair<std::string, std::unique_ptr<AbstractAlgorithm>>> houseAlgoPairs;
        AlgorithmRegistrar &registrar = AlgorithmRegistrar::getAlgorithmRegistrar();

        // Create all possible pairs
        for (const auto &house : houseFiles) {
            for (const auto &algoFactoryPair : registrar) {
                houseAlgoPairs.emplace_back(house, algoFactoryPair.create());
            }
        }

        std::queue<std::pair<std::string, std::unique_ptr<AbstractAlgorithm>>> tasks;
        for (auto &pair : houseAlgoPairs) {
            tasks.push(std::move(pair));
        }

        std::mutex queueMutex;
        std::vector<std::thread> threads;

        for (int i = 0; i < numOfThreads; ++i) {
            threads.emplace_back(worker, std::ref(tasks), std::ref(queueMutex));
        }

        // Join the threads
        for (auto &t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }

        // Close all opened .so files
        for (void *handle : algoHandles) {
            if (handle) {
                dlclose(handle);
            }
        }

    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
        return 1;
    }
    return 0;
}
