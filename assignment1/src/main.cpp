#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>
#include <filesystem>

#include "../include/InputGenerator.h"
#include "../include/House.h"
#include "../include/VacuumCleaner.h"
#include "../include/Algorithm.h"

// Compile with:
// g++ -std=c++20 -Wall -Wextra -Werror -pedantic src/main.cpp src/House.cpp src/VacuumCleaner.cpp src/Algorithm.cpp src/InputGenerator.cpp -o myrobot

using namespace std;

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            throw runtime_error("Usage: myrobot <house_input_file>");
        }

        // Generate the house and read from the input file
//        InputGenerator::generate_random_house(argv[1]);
        string house_input_file = argv[1];
        House house(house_input_file);
        size_t pos = house_input_file.rfind("/");
        std::string input_file_name = (pos == std::string::npos) ? house_input_file : house_input_file.substr(pos + 1);

        pair<int, int> docking_station = house.get_docking_station();

        int max_battery_steps = house.get_max_battery_steps();
        int max_steps_allowed = house.get_max_steps_allowed();

        VacuumCleaner vacuum_cleaner(house);

        vacuum_cleaner.run();

        string output_file = "output_" + input_file_name;
        ofstream outfile(output_file);
        if (!outfile.is_open()) {
            throw runtime_error("Could not open output file: " + output_file);
        }

        outfile << vacuum_cleaner.get_assignment_log() << endl;
        outfile << "Total steps: " << vacuum_cleaner.get_total_steps_taken() << endl;
        outfile << "Total dirt left: " << house.get_total_dirt() << endl;
        outfile << "Battery exhausted: " << (vacuum_cleaner.battery_exhausted() ? "Yes" : "No") << endl;
        outfile << "Mission success: " << (vacuum_cleaner.mission_completed(house) ? "Yes" : "No") << endl;
        outfile.close();

    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
    } catch (...) {
        cerr << "Unknown exception occurred." << endl;
    }

    return 0;
}
