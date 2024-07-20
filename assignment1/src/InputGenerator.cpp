#include "../include/InputGenerator.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

SurroundingType InputGenerator::get_random_surrounding_type() {
    int rand_val = rand() % 2;
    return static_cast<SurroundingType>(rand_val);
}

HouseShape InputGenerator::get_random_house_shape() {
    int rand_val = rand() % 2;
    return static_cast<HouseShape>(rand_val);
}

void InputGenerator::add_full_surround(vector<vector<char>>& layout) {
    int rows = layout.size();
    int cols = 0;

    for (const auto& row : layout) {
        if (int(row.size()) > cols) {
            cols = row.size();
        }
    }

    vector<vector<char>> new_layout(rows + 2, vector<char>(cols + 2, '#'));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < int(layout[i].size()); ++j) {
            new_layout[i + 1][j + 1] = layout[i][j];
        }
    }
    layout = new_layout;
}

void InputGenerator::create_non_rectangle_layout(vector<vector<char>>& layout, int rows, int cols) {
    layout.clear();
    for (int i = 0; i < rows; ++i) {
        int row_length = rand() % (cols - 1) + 2; // Ensure each row has at least 2 columns
        vector<char> row(row_length, ' ');
        layout.push_back(row);
    }
}

void InputGenerator::generate_random_house(const string& filename) {
    srand(time(0));  // Seed for random number generation

    // Random values for max_steps and max_battery_steps
    int max_steps = rand() % 251 + 50;  // Random value between 50 and 300
    int max_battery_steps = rand() % 81 + 20;  // Random value between 20 and 100

    // Random house dimensions
    int rows = rand() % 9 + 2;  // Random value between 2 and 10
    int cols = rand() % 19 + 2;  // Random value between 2 and 20

    // Create an empty house layout
    vector<vector<char>> layout;
    HouseShape house_shape = get_random_house_shape();
    if (house_shape == RECTANGLE) {
        layout = vector<vector<char>>(rows, vector<char>(cols, ' '));
    } else {
        create_non_rectangle_layout(layout, rows, cols);
    }

    // Place random internal walls
    int num_walls = (rows * cols) * 33 / 100;  // 33% of the house
    for (int i = 0; i < num_walls; ++i) {
        int wall_row = rand() % layout.size();
        int wall_col = rand() % layout[wall_row].size();
        layout[wall_row][wall_col] = '#';
    }

    // Place random dirt levels
    int num_dirt_cells = (rows * cols) * 33 / 100;  // 33% of the house
    for (int i = 0; i < num_dirt_cells; ++i) {
        int dirt_row = rand() % layout.size();
        int dirt_col = rand() % layout[dirt_row].size();
        if (layout[dirt_row][dirt_col] == ' ' || layout[dirt_row][dirt_col] == 'D') {
            layout[dirt_row][dirt_col] = '1' + rand() % 9;  // Dirt level between 1 and 9
        }
    }

    // Place the docking station
    int dock_row, dock_col;
    do {
        dock_row = rand() % layout.size();
        dock_col = rand() % layout[dock_row].size();
    } while (layout[dock_row][dock_col] != ' ');  // Ensure it is placed in an empty cell
    layout[dock_row][dock_col] = 'D';

    // Randomly choose how to surround the house with walls
    SurroundingType surround_type = get_random_surrounding_type();
    if (surround_type == FULLY_SURROUND) {
        add_full_surround(layout);
    }

    // Write the layout to a file
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file for writing!" << endl;
        return;
    }

    file << "max_steps = " << max_steps << "\n";
    file << "max_battery_steps = " << max_battery_steps << "\n\n";

    for (const auto& row : layout) {
        for (const auto& cell : row) {
            file << cell;
        }
        file << "\n";
    }

    file.close();
}
