#include "../include/Algorithm.h"
#include <cstdlib>
#include <utility>
#include <stdexcept>
#include <iostream>

using namespace std;

void Algorithm::set_sensors(const DirtSensor* dirt, const WallSensor* wall, const BatterySensor* battery) {
    dirt_sensor = dirt;
    wall_sensor = wall;
    battery_sensor = battery;
}

string Algorithm::get_next_move() {
    if (dirt_sensor->get_dirt_level() > 0 && battery_sensor->get_battery_level() > int(path_back_to_docking.size())) {
        trip_steps += 1;
        return "Stay"; // Clean the spot
    }

    // Check battery level to decide when to go back to the docking station
    if (battery_sensor->get_battery_level() <= int(path_back_to_docking.size()) + 1 || path_back_to_docking.empty()) {
        // Get back to docking station
        if (!path_back_to_docking.empty()) {
            if (curr_position == docking_station) {
                path_back_to_docking.clear();
                return "Stay";
            }

            string direction = path_back_to_docking.front();
            path_back_to_docking.pop_front();

            // Update current position
            curr_position.first += direction_map[direction].first;
            curr_position.second += direction_map[direction].second;

            return direction;
        } else {
            // Return "Stay" to indicate the need to charge once at the docking station
            if (curr_position == docking_station) {
                if (battery_sensor->get_battery_level() < max_battery_steps) {
                    return "Stay";
                } else {
                    trip_steps = 0;
                }
            }
        }
    }

    vector<string> possible_moves = {"North", "East", "South", "West"};
    vector<string> valid_moves;

    for (const string& direction : possible_moves) {
        if (!wall_sensor->surrounding_walls().at(direction)) {
            valid_moves.push_back(direction);
        }
    }

    if (valid_moves.empty()) {
        throw runtime_error("Vacuum cleaner is stuck, no valid moves available.");
    }

    // Randomly choose a valid move
    string direction = valid_moves[rand() % valid_moves.size()];

    // Update current position
    curr_position.first += direction_map[direction].first;
    curr_position.second += direction_map[direction].second;

    // Increase current trip steps
    trip_steps += 1;

    // Remember the path to find the way back to the docking station
    path_back_to_docking.push_front(opp_direction[direction]);

    return direction;
}

