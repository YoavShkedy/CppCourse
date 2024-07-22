#include "../include/utils.h"
#include "../include/Algorithm.h"
#include "../include/Simulator.h"

int main(int argc, char **argv) {
    try {
        if (argc < 2) {
            throw std::runtime_error("Usage: myrobot <house_input_file>");
        }
        std::string houseFilePath = argv[1];
        Simulator simulator;
        simulator.readHouseFile(houseFilePath);
        Algorithm algo;
        simulator.setAlgorithm(algo);

        //simulator.run();
        simulator.runWithSim();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
    }
    return 0;
}
