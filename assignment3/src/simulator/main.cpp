#include "utils.h"
#include "AlgorithmRegistration.h"
#include "Simulator.h"
#include "../Algorithm_206448649/Algorithm_206448649_314939398_A.h"
#include "../Algorithm_314939398/Algorithm_314939398.h"

#include <filesystem>
#include <dlfcn.h>

void register_libs(const char *libs_dir, std::vector<void *> &libs) {
    for (auto &dirent: std::filesystem::directory_iterator(std::filesystem::path(libs_dir))) {
        if (dirent.path().extension() == ".so") {
            void *lib_ptr = dlopen(dirent.path().c_str(), RTLD_LAZY);
            if (lib_ptr == nullptr) {
                throw std::runtime_error("Error: lib_ptr == nulptr");
            }
            libs.push_back(lib_ptr);
        }
    }
}

void clear_libs(std::vector<void *> &libs) {
    for (void *lptr: libs) {
        dlclose(lptr);
    }
}

int main(int argc, char **argv) {
    try {
        //TODO
        std::vector<void *> algo_libs;
        register_libs(argv[1], algo_libs);

        if (argc != 3) {
            throw std::runtime_error("Usage: myrobot house_path=<house_path>, algo_path=<algo_path>");
        }
        std::string houseDirPath;
        std::string algoDirPath;
        for (int i = 1; i < 3; i++) {
            std::istringstream ss(argv[i]);
            std::string key;
            std::string path;
            // Extract key and path
            if (std::getline(ss, key, '=') && std::getline(ss, path)) {
                key.erase(key.find_last_not_of(" \t\n\r\f\v") + 1); // Trim trailing spaces
                path.erase(0, path.find_first_not_of(" \t\n\r\f\v")); // Trim leading spaces

                if (key == "house_path") {
                    houseDirPath = path;
                } else if (key == "algo_path") {
                    algoDirPath = path;
                } else {
                    throw std::runtime_error("Invalid argument key: " + key);
                }
            } else {
                throw std::runtime_error("Invalid argument format: " + std::string(argv[i]));
            }
        }
        std::vector<std::string> houseFiles;
        std::vector<std::string> algoFiles;

        // Iterate over houseDirPath to find .house files
        for (const auto& entry : std::filesystem::directory_iterator(houseDirPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".house") {
                houseFiles.push_back(entry.path().string());
            }
        }

        // Iterate over algoDirPath to find .so files
        for (const auto& entry : std::filesystem::directory_iterator(algoDirPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".so") {
                algoFiles.push_back(entry.path().string());
            }
        }

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
    }

    return 0;
}
