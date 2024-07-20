#include <iostream>
#include <string>
#include "../include/Algorithm.h"
#include "../include/Simulator.h"

int main(int argc, char **argv) {
    // Set the global log level to debug
    spdlog::set_level(spdlog::level::debug);

    // Create console and file sinks
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/debug.log", true);

    // Combine the sinks
    std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};
    auto logger = std::make_shared<spdlog::logger>("multi_sink", sinks.begin(), sinks.end());

    // Set the logger to flush on debug level
    logger->flush_on(spdlog::level::debug);

    // Set the combined logger as the default logger
    spdlog::set_default_logger(logger);

    // Log a startup message
    spdlog::info("\nLogger initialized and set as default\n");
    spdlog::default_logger()->flush(); // Ensure log is flushed

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
