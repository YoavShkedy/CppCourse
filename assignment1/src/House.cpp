#include "../include/House.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

// Constructor implementation
House::House(const string &filename) {
    init_house(filename);
    ensure_walls();
}

void House::init_house(const string &filename) {
    ifstream file(filename);
    if (!file.is_open())
        throw runtime_error("Could not open file");

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue; // Skip empty lines
        if (line[0] == 'm') {
            // Process the line as configuration setting
            // Example: max_steps_allowed: 100
            istringstream ss(line);
            string key;
            int value;

            // Read up to the colon
            getline(ss, key, '=');
            // Remove any leading whitespace from the value part
            ss >> value;
            // Trim any trailing whitespace from the key
            key.erase(key.find_last_not_of(" \t\n\r\f\v") + 1);

            if (key == "max_steps")
                max_steps_allowed = value;
            else if (key == "max_battery_steps")
                max_battery_steps = value;
            else
                throw runtime_error("Unknown configuration setting.");
        } else {
            // Process the line as part of the house layout
            deque<char> row(line.begin(), line.end());
            // Check for max column size
            if (row.size() > layout_cols_num) layout_cols_num = row.size();
            // Add row to layout
            layout.push_back(row);

            // Check if the line contains the docking station 'D'
            string::size_type pos = line.find('D');
            if (pos != string::npos) {
                if (!docking_station_found) {
                    // Record the coordinates of the docking station
                    docking_station = make_pair(static_cast<int>(layout.size() - 1), static_cast<int>(pos));
                    docking_station_found = true;
                } else
                    throw runtime_error("Multiple docking stations defined.");
            }
            // Update total dirt and dirty positions
            for (char c : row) {
                if (c >= '1' && c <= '9') {
                    int dirt_level = c - '0';
                    total_dirt += dirt_level;
                }
            }
        }
        layout_rows_num++;
    }
    if (!docking_station_found)
        throw runtime_error("Docking station not found.");
    else if (max_steps_allowed <= 0 || max_battery_steps <= 0)
        throw runtime_error("max_steps_allowed a max_battery_steps must be positive numbers.");
}

void House::pad_layout(){
    for (auto &row: layout)
        if (row.size() < layout_cols_num) row.resize(layout_cols_num, '#');
}

void House::ensure_walls() {
    // Sanity check
    unsigned long rows_num = layout.size();
    if (rows_num == 0) // No layout to process
        throw runtime_error("No house layout.");

    // Ensure northern and southern walls
    deque<char> wall(layout_cols_num, '#');
    if (layout[0] != wall) layout.insert(layout.begin(), wall);
    if (layout.back() != wall) layout.push_back(wall);
    if (layout.size() > layout_rows_num) layout_rows_num = layout.size();

    // Ensure eastern and western walls
    for (auto &row: layout) {
        if (row.back() != '#') row.push_back('#');
        if (row.front() != '#') row.push_front('#');
        if (row.size() > layout_cols_num) layout_cols_num = row.size();
    }

    // Pad layout with '#' to reshape it to a rectangle
    pad_layout();

    // Find new docking station spot
    // Update docking station position after ensuring walls
    for (int i = 0; i < int(layout.size()); ++i) {
        for (int j = 0; j < int(layout[i].size()); ++j) {
            if (layout[i][j] == 'D') {
                docking_station = make_pair(i, j);
                return;
            }
        }
    }
}

void House::print_house() const {
    for (const auto &row: layout) {
        for (const auto &cell: row)
            cout << cell;
        cout << endl;
    }
}

int House::get_dirt_level(int x, int y) const {
    if (x >= int(layout_rows_num) || y >= int(layout_cols_num) || x < 0 || y < 0)
        return 0;
    if (layout[x][y] >= '1' && layout[x][y] <= '9')
        return layout[x][y] - '0';
    return 0;
}

bool House::is_wall(int x, int y) const {
    if (x >= int(layout_rows_num) || y >= int(layout_cols_num) || x < 0 || y < 0)
        return true;
    return layout[x][y] == '#';
}

pair<int, int> House::get_docking_station() const {
    return docking_station;
}

vector<deque<char>> House::get_layout() const {
    return layout;
}

int House::get_max_steps_allowed() const {
    return max_steps_allowed;
}

int House::get_max_battery_steps() const {
    return max_battery_steps;
}

int House::get_total_dirt() const {
    return total_dirt;
}

void House::decrease_dirt(int x, int y) {
    if (x >= layout_rows_num || y >= layout_cols_num || x < 0 || y < 0)
        throw runtime_error("Error: Position out of layout bounds");
    else if (layout[x][y] == '#')
        throw runtime_error("Error: Cannot decrease dirt of walls");
    else if (layout[x][y] == 'D')
        throw runtime_error("Error: Cannot decrease dirt of docking station");
    else if (layout[x][y] == ' ')
        throw runtime_error("Error: Cannot decrease dirt of clean position");


    int dirt_level = layout[x][y] - '0';
    if (dirt_level == 0) return;
    int tmp = layout[x][y] - '0';
    tmp--;
    layout[x][y] = tmp + '0';
    decrease_total_dirt();
}

void House::decrease_total_dirt() {
    if (total_dirt == 0) return;
    total_dirt--;
}
