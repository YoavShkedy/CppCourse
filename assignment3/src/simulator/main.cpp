#include "utils.h"
#include "Simulator.h"
#include "Algorithm_a.h"
#include "Algorithm_b.h"

int main(int argc, char **argv) {
    try {
        if (argc < 2) {
            throw std::runtime_error("Usage: myrobot <house_input_file>");
        }
        std::string houseFilePath = argv[1];
        Simulator simulator;
        simulator.readHouseFile(houseFilePath);
        auto algo_b = std::make_unique<Algorithm_b>();
        simulator.setAlgorithm(std::move(algo_b));

//        simulator.run();
        simulator.runWithSim();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
    }
    return 0;
}
