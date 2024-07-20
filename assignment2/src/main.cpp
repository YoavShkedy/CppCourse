#include <iostream>
#include <string>
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
        simulator.printHouseLayout();
        std::cout << '\n';

//        simulator.run();
        simulator.runWithSim();

        std::string dockingStationPosition =
                "(" + std::to_string(simulator.getSimDockingStationPosition().first) + ", " +
                std::to_string(simulator.getSimDockingStationPosition().second) + ")";
        std::string endPosition = "(" + std::to_string(simulator.getSimCurrPosition().first) + ", " +
                                  std::to_string(simulator.getSimCurrPosition().second) + ")";

        std::cout << '\n' << "Total dirt level: " << simulator.getTotalDirt() << std::endl;
        std::cout << "End position: " << (endPosition == dockingStationPosition ? "Docking station" : endPosition)
                  << std::endl;
    }
    catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
    }

    return 0;
}
