#include "../include/VacuumCleaner.h"
#include "../include/Algorithm.h"

#include <vector>
#include <string>
#include <utility>
#include <map>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

using namespace std;
// Constructor
VacuumCleaner::VacuumCleaner(House& house)
        : house(house), max_battery_steps(house.get_max_battery_steps()), max_steps_allowed(house.get_max_steps_allowed()),
          docking_station_position(house.get_docking_station()), algorithm(docking_station_position, max_battery_steps) {
    position = docking_station_position;
    battery_level = max_battery_steps;
    current_trip_steps = 0;
    total_steps_taken = 0;
    algorithm.set_sensors(new Sensors(*this), new Sensors(*this), new Sensors(*this));
}

void VacuumCleaner::run() {
    if (get_total_steps_taken() > house.get_max_steps_allowed())
        throw runtime_error("Mission failed: Used maximum number of steps allowed.");

    while (!mission_completed(house) && total_steps_taken < max_steps_allowed) {
        string action = algorithm.get_next_move();
        if (action == "Stay") {
            if (position == docking_station_position) {
                charge();
            } else {
                clean();
            }
        } else {
            move(action);
        }
    }
}

void VacuumCleaner::run_with_sim() {
    if (get_total_steps_taken() > house.get_max_steps_allowed())
        throw runtime_error("Mission failed: Used maximum number of steps allowed.");

    // Initial print
    print_house("Starting");

    while (!mission_completed(house) && total_steps_taken < max_steps_allowed) {
        string action = algorithm.get_next_move();
        if (action == "Stay") {
            if (position == docking_station_position) {
                charge();
            } else {
                clean();
            }
        } else {
            move(action);
        }
        print_house(action);
        this_thread::sleep_for(chrono::seconds(1));
    }

    print_house("Mission Completed");
}

void VacuumCleaner::print_house(const string& action) const {
    system("clear"); // For Linux and macOS. Use "CLS" for Windows.
    cout << "Max steps allowed: " << max_steps_allowed << endl;
    cout << "Total Steps Taken: " << total_steps_taken << "\n" << endl;
    cout << "Action: " << action << endl;
    cout << "Battery Level: " << battery_level << endl;
    cout << "Current Trip Steps: " << current_trip_steps << endl;

    vector<deque<char>> layout = house.get_layout();
    if (action == "Starting") {
        layout[docking_station_position.first][docking_station_position.second] = 'V';
    } else {
        layout[position.first][position.second] = 'V';
    }

    for (const auto &row: layout) {
        for (const auto &cell: row)
            cout << cell << " ";
        cout << endl;
    }
    cout << endl;
}

int VacuumCleaner::Sensors::get_dirt_level() const {
    return vc.house.get_dirt_level(vc.position.first, vc.position.second);
}

map<string, bool> VacuumCleaner::Sensors::surrounding_walls() const {
    int x = vc.position.first;
    int y = vc.position.second;
    map<string, bool> walls;
    walls["North"] = vc.house.is_wall(x - 1, y);
    walls["South"] = vc.house.is_wall(x + 1, y);
    walls["East"] = vc.house.is_wall(x, y + 1);
    walls["West"] = vc.house.is_wall(x, y - 1);
    return walls;
}

int VacuumCleaner::Sensors::get_battery_level() const {
    return vc.battery_level;
}

void VacuumCleaner::move(string direction) {
    if (direction != "North" &&
        direction != "South" &&
        direction != "East" &&
        direction != "West") {
        stringstream ss;
        ss << direction << " is an illegal direction.";
        throw runtime_error(ss.str());
    }

    if (battery_exhausted()) { // Should never happen
        throw runtime_error("Battery exhausted and vacuum cleaner not in docking station.");
    }

    assignment_log.push_back(string("Move: ") + direction);

    if (direction == "North") position.first -= 1;
    if (direction == "South") position.first += 1;
    if (direction == "East") position.second += 1;
    if (direction == "West") position.second -= 1;

    battery_level--;
    current_trip_steps++;
    total_steps_taken++;
}

void VacuumCleaner::clean() {
    int x = position.first;
    int y = position.second;
    house.decrease_dirt(x, y);
    assignment_log.emplace_back("Clean");
    battery_level--;
    current_trip_steps++;
    total_steps_taken++;
}

bool VacuumCleaner::battery_exhausted() const {
    if (battery_level < 0) // Should never happen
        throw runtime_error("Negative battery level.");
    return battery_level == 0;
}

bool VacuumCleaner::mission_completed(const House &house) const {
    return house.get_total_dirt() == 0 && position == docking_station_position;
}

string VacuumCleaner::get_assignment_log() const {
    string result;
    for (const string &s: assignment_log)
        result += s + "\n";
    return result;
}

pair<int, int> VacuumCleaner::get_position() const {
    return position;
}

// charge() should be called ONLY ONCE when the VacuumCleaner gets to the docking station
void VacuumCleaner::charge() {
    if (position != docking_station_position)
        throw runtime_error("Cannot charge away from docking station.");

    if (position == docking_station_position) {
        if (battery_level < max_battery_steps) {
            battery_level++;
            total_steps_taken++;
            assignment_log.emplace_back("Charge");
        }
        if (battery_level >= max_battery_steps) {
            battery_level = max_battery_steps;
            current_trip_steps = 0; // Reset current trip steps
        }
    }
}

int VacuumCleaner::get_current_trip_steps() const {
    return current_trip_steps;
}

int VacuumCleaner::get_total_steps_taken() const {
    return total_steps_taken;
}
