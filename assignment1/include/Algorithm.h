#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Sensor.h"

#include <string>
#include <vector>
#include <deque>
#include <utility>
#include <map>

using namespace std;

class Algorithm {
public:
    Algorithm(const pair<int, int>& docking_station, int max_battery_steps) :
    docking_station(docking_station), curr_position(docking_station), max_battery_steps(max_battery_steps) {
        trip_steps = 0;
    }

    void set_sensors(const DirtSensor* dirt, const WallSensor* wall, const BatterySensor* battery);

    string get_next_move();

private:
    const DirtSensor* dirt_sensor;
    const WallSensor* wall_sensor;
    const BatterySensor* battery_sensor;

    int trip_steps;
    pair<int, int> docking_station;
    pair<int, int> curr_position;
    deque<string> path_back_to_docking;
    int max_battery_steps;

    map<string, string> opp_direction = {
            {"North", "South"},
            {"South", "North"},
            {"East", "West"},
            {"West", "East"}
    };

    map<string, pair<int, int>> direction_map = {
            {"North", {-1, 0}},
            {"East", {0, 1}},
            {"South", {1, 0}},
            {"West", {0, -1}}
    };
};

#endif // ALGORITHM_H
