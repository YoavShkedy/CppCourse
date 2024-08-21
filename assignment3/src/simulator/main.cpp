#include "utils.h"
#include "AlgorithmRegistration.h"
#include "Simulator.h"
#include "../Algorithm_206448649/Algorithm_206448649.h"
#include "../Algorithm_314939398/Algorithm_314939398.h"

#include <filesystem>
#include <dlfcn.h>

void register_libs(const char *libs_dir, std::vector<void *> &libs) {
    for (auto &dirent: std::__fs::filesystem::directory_iterator(std::__fs::filesystem::path(libs_dir))) {
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
        std::vector<void *> algo_libs;
        register_libs(argv[1], algo_libs);
        if (argc < 3) {
            throw std::runtime_error("Usage: myrobot <house_path>, <algorithm_path>");
        }
        std::string houseFilePath = argv[1];
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
