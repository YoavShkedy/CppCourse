#include "utils.h"
#include "AlgorithmRegistration.h"
#include "Simulator.h"

std::string houseDirPath = "";
std::string algoDirPath = "";
int numOfThreads = 10; // default value
bool summaryOnly = false;

void print(std::string s) {
    std::cout << s << std::endl;
}

void handleCommandLineArguments(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-summary_only") {
            summaryOnly = true;
        } else {
            std::istringstream ss(arg);
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
                } else {
                    throw std::runtime_error("Invalid argument key: " + key);
                }
            } else {
                throw std::runtime_error("Invalid argument format: " + arg);
            }
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


void runSim(Simulator *simulator, std::atomic<bool>* finished, std::condition_variable* cv_timeout, std::mutex* m, int* score) {
    // Making the thread cancelable
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, nullptr);

    std::unique_lock<std::mutex> lock(*m); // Prevent the simulation thread from running before the main thread starts waiting
    lock.unlock(); // Simulation thread should not hold the lock when executing the simulation

    // Run the simulation and save the score
    *score = simulator->run();

    // Indicate that simulation has finished running
    lock.lock();
    *finished = true;
    lock.unlock();

    // Notify the main thread that the simulation is done
    cv_timeout->notify_one();
}

int runWrapper(std::pair<std::string, std::unique_ptr<AbstractAlgorithm>> houseAlgoPair) {
    const std::string &houseFilePath = houseAlgoPair.first;

    // Get the algo name
    const std::type_info& type_info = typeid(houseAlgoPair.second);
    int status;
    char *demangled = abi::__cxa_demangle(type_info.name(), nullptr, nullptr, &status);
    std::string algo_name = (status == 0) ? demangled : type_info.name();
    free(demangled);

    std::unique_ptr<AbstractAlgorithm> algo = std::move(houseAlgoPair.second);

    Simulator simulator(summaryOnly);
    simulator.readHouseFile(houseFilePath);
    simulator.setAlgorithm(std::move(algo));

    int maxSteps = simulator.getMaxSteps();
    int initialDirt = simulator.getInitialDirt();
    int score = -1;  // Initialize score to an error value or default

    std::atomic<bool> finished = false;
    std::condition_variable cv_timeout;
    std::mutex m;

    // Prevent the simulation thread from running before the main thread starts waiting
    std::unique_lock<std::mutex> lock(m);

    // Create the simulation thread
    std::thread simThread(runSim, &simulator, &finished, &cv_timeout, &m, &score);

    // Put the main thread into waiting state and release the mutex
    // Wait until cv notification or timeout has passed
    if (!cv_timeout.wait_for(lock, std::chrono::milliseconds(maxSteps), [&finished]() { return finished.load(); })) {
        // Thread running the simulation has reached its timeout
        // Main thread reacquires the mutex
        pthread_cancel(simThread.native_handle()); // Cancel the simulation thread
        simThread.detach(); // Detach simulation thread, cleaning up without blocking the main thread

        // Calculate the timeout score
        int timeoutScore = (maxSteps * 2) + (initialDirt * 300) + 2000;

        // Create an error file which notifies about the timeout
        std::string errorFileName = algo_name + ".error";
        writeError(errorFileName, "Timout has occurred running " + algo_name + " on " + houseFilePath);

        // Create the timeout output file and return the timeout score
        simulator.createTimeoutOutputFile(timeoutScore);
        lock.unlock();
        return timeoutScore;
    } else {
        // (Received cv notification) or (timeout has not passed and finished == true)
        simThread.join();
        return score;  // Return the score obtained from the simulation
    }
}

void worker(std::queue<std::pair<std::string, std::unique_ptr<AbstractAlgorithm>>> &tasks, std::mutex &queueMutex,
            std::map<std::string, std::map<std::string, int>> &results,
            std::mutex &resultsMutex) {
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

        // Extract the file name from the full path
        std::filesystem::path housePath(task.first);
        std::string houseFileName = housePath.filename().string();

        // Capture the algorithm name before moving the task
        AbstractAlgorithm& algo_ref = *task.second;
        const std::type_info& type_info = typeid(algo_ref);

        int status;
        char *demangled = abi::__cxa_demangle(type_info.name(), nullptr, nullptr, &status);
        std::string algo_name = (status == 0) ? demangled : type_info.name();
        free(demangled);  // Remember to free the demangled name

        int score = runWrapper(std::move(task));
        {
            std::lock_guard<std::mutex> lock(resultsMutex);
            results[houseFileName][algo_name] = score;
        }
    }
}


void writeCSV(const std::string &filename,
              const std::set<std::string> &algorithms,
              const std::vector<std::string> &houses,
              const std::map<std::string, std::map<std::string, int>> &results) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open CSV file: " << filename << std::endl;
        return;
    }

    // Write the header
    file << "Algorithm/House";
    for (const auto &house : houses) {
        // Extract the file name from the full path
        std::filesystem::path housePath(house);
        std::string houseFileName = housePath.filename().string();
        file << "," << houseFileName;
    }
    file << "\n";

    // Write the data
    for (const auto &algorithm : algorithms) {
        file << algorithm;
        for (const auto &house : houses) {
            // Extract the file name from the full path
            std::filesystem::path housePath(house);
            std::string houseFileName = housePath.filename().string();

            auto algoIt = results.find(houseFileName);
            if (algoIt != results.end()) {
                auto houseIt = algoIt->second.find(algorithm);
                if (houseIt != algoIt->second.end()) {
                    file << "," << houseIt->second;
                } else {
                    file << ",";
                }
            } else {
                file << ",";
            }
        }
        file << "\n";
    }
}

void checkHouseFiles(std::vector<std::string> &houseFiles) {
    // Iterate over houseDirPath to find .house files
    for (const auto &entry: std::filesystem::directory_iterator(houseDirPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".house") {
            // Try to open the .house file
            std::ifstream currHouseFile(entry.path());
            if (!currHouseFile.is_open()) {
                std::string errorFileName = entry.path().stem().string() + ".error";
                writeError(errorFileName, "Failed to open house file: " + entry.path().string());
                continue; // Skip this file
            }
            // Test if the house file is valid using readHouseFile
            Simulator sim;
            bool validHouseFile = sim.readHouseFile(entry.path());
            if (!validHouseFile) {
                std::string errorFileName = entry.path().stem().string() + ".error";
                writeError(errorFileName, "Invalid house file: " + entry.path().string());
                continue; // Skip this file
            }
            // If the file opened successfully and is valid, add it to the houseFiles vector
            houseFiles.push_back(entry.path().string());

            if (houseFiles.empty()) {
                throw std::runtime_error("Cannot find .house files in " + std::filesystem::current_path().string());
            }
            // Close the file
            currHouseFile.close();
        }

        if (houseFiles.empty()) {
            throw std::runtime_error("Cannot find .house files in " + std::filesystem::current_path().string());
        }

    }
}

void checkAlgorithmFiles(std::vector<void *> &algoHandles) {
    // Iterate over algoDirPath to find .so files and open them with dlopen
    for (const auto &entry: std::filesystem::directory_iterator(std::filesystem::path(algoDirPath))) {
        if (entry.is_regular_file() &&
            (entry.path().extension() == ".so" || entry.path().extension() == ".dylib")) {
            void *handle = dlopen(entry.path().c_str(), RTLD_LAZY);
            if (!handle) {
                std::string errorFileName = entry.path().stem().string() + ".error";
                writeError(errorFileName,
                           "Failed to open algorithm file: " + entry.path().string() + "\ndlerror: " + dlerror());
                continue; // Skip this file
            }
            // Store the handle for later dlclose
            algoHandles.push_back(handle);
        }

        if (algoHandles.empty()) {
            throw std::runtime_error("Cannot find .so files in " + std::filesystem::current_path().string());
        }

    }
}

void createHouseAlgoPairs(std::vector<std::string> &houseFiles, std::set<std::string> &algorithms, std::vector<std::pair<std::string, std::unique_ptr<AbstractAlgorithm>>> &houseAlgoPairs) {
    // Create all possible pairs
    for (const auto &house: houseFiles) {
        for (const auto& algoFactoryPair : AlgorithmRegistrar::getAlgorithmRegistrar()) {
            // Create the algorithm registrar
            auto algorithm = algoFactoryPair.create();

            // Check if the algorithm creation failed (returns nullptr)
            if (algorithm) {
                // Only add the pair if the algorithm is valid (non-null)
                houseAlgoPairs.emplace_back(house, std::move(algorithm));
                algorithms.insert(algoFactoryPair.name()); // using a set to prevent duplications
            } else {
                std::string errorFileName = algoFactoryPair.name() + ".error";
                writeError(errorFileName, "Algorithm Factory returned a nulptr for: " + algoFactoryPair.name());
                break; // do not create house<->Algo pairs with this algorithm
            }
        }
    }
}


int main(int argc, char **argv) {
    try {
        if (argc > 5) {
            throw std::runtime_error("up to 5 command-line arguments allowed");
        }
        handleCommandLineArguments(argc, argv);

        std::vector<std::string> houseFiles;
        checkHouseFiles(houseFiles);

        AlgorithmRegistrar::getAlgorithmRegistrar().clear();

        std::vector<void *> algoHandles;
        std::set<std::string> algorithms;
        checkAlgorithmFiles(algoHandles);

        // Vector to store pairs of house files and algorithm handles
        std::vector<std::pair<std::string, std::unique_ptr<AbstractAlgorithm>>> houseAlgoPairs;
        createHouseAlgoPairs(houseFiles, algorithms, houseAlgoPairs);

        std::queue<std::pair<std::string, std::unique_ptr<AbstractAlgorithm>>> tasks;
        for (auto &pair: houseAlgoPairs) {
            tasks.push(std::move(pair));
        }

        std::mutex queueMutex;
        std::mutex resultsMutex;
        std::vector<std::thread> threads;
        std::map<std::string, std::map<std::string, int>> results;

        for (int i = 0; i < numOfThreads; ++i) {
            threads.emplace_back(worker, std::ref(tasks), std::ref(queueMutex), std::ref(results), std::ref(resultsMutex));
        }

        // Join the threads
        for (auto &t: threads) {
            if (t.joinable()) {
                t.join();
            }
        }

        // Write results to CSV
        writeCSV("summary.csv", algorithms, houseFiles, results);

        // Clear the AlgorithmRegistrar to remove all registered algorithms
        AlgorithmRegistrar::getAlgorithmRegistrar().clear();

        // Close all opened .so files
        for (void *handle: algoHandles) {
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
