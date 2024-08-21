#include "utils.h"
#include "AlgorithmRegistration.h"
#include "Simulator.h"
#include "../Algorithm_206448649/Algorithm_206448649_314939398_A.h"
#include "../Algorithm_314939398/Algorithm_314939398.h"

#include <filesystem>
#include <dlfcn.h>


void writeError(const std::string& fileName, const std::string& errorMessage) {
    std::ofstream errorFile(fileName);
    if (errorFile.is_open()) {
        errorFile << errorMessage << std::endl;
        errorFile.close();
    } else {
        std::cerr << "Failed to open error file: " << fileName << std::endl;
    }
}

int main(int argc, char **argv) {
    try {
        //TODO: up to 5 arguments allowed
        if (argc != 3) {
            throw std::runtime_error("Usage: myrobot -house_path=<house_path> -algo_path=<algo_path>");
        }

        std::string houseDirPath;
        std::string algoDirPath;

        //TODO: add support to -num_threads -summary_only
        for (int i = 1; i < 3; i++) {
            std::istringstream ss(argv[i]);
            std::string key;
            std::string path;

            // Extract key and path
            if (std::getline(ss, key, '=') && std::getline(ss, path)) {
                key.erase(key.find_last_not_of(" \t\n\r\f\v") + 1); // Trim trailing spaces
                path.erase(0, path.find_first_not_of(" \t\n\r\f\v")); // Trim leading spaces

                if (key == "-house_path") {
                    houseDirPath = path;
                } else if (key == "-algo_path") {
                    algoDirPath = path;
                } else {
                    throw std::runtime_error("Invalid argument key: " + key);
                }
            } else {
                throw std::runtime_error("Invalid argument format: " + std::string(argv[i]));
            }
        }

        std::vector<std::string> houseFiles;
        std::vector<void *> algoHandles;

        // Iterate over houseDirPath to find .house files
        for (const auto& entry : std::filesystem::directory_iterator(houseDirPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".house") {
                houseFiles.push_back(entry.path().string());
                // Open the .house file
                std::ifstream currHouseFile(entry.path());
                if (!currHouseFile.is_open()) {
                    std::string errorFileName = entry.path().stem().string() + ".error";
                    writeError(errorFileName, "Failed to open house file: " + entry.path().string());
                    continue; // Skip this file
                }
                // Close the file
                currHouseFile.close();
            }
        }
        // Iterate over algoDirPath to find .so files and open them with dlopen
        for (const auto& entry : std::filesystem::directory_iterator(algoDirPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".so") {
                void *handle = dlopen(entry.path().c_str(), RTLD_LAZY);
                if (!handle) {
                    std::string errorFileName = entry.path().stem().string() + ".error";
                    writeError(errorFileName, "Failed to open algorithm file: " + entry.path().string() + "\ndlerror: " + dlerror());
                    continue; // Skip this file
                }
                // Store the handle for later dlclose
                algoHandles.push_back(handle);
            }
        }

        /*
        Simulator simulator;
        simulator.readHouseFile(houseFilePath);
        auto algo_b = std::make_unique<Algorithm_314939398>();
        simulator.setAlgorithm(std::move(algo_b));

//        simulator.run();
        simulator.runWithSim();
        clear_libs(algo_libs);
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
    }*/

        // Close all opened .so files
        for (void *handle : algoHandles) {
            if (handle) {
                dlclose(handle);
            }
        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
