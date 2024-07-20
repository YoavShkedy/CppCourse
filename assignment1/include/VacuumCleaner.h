#ifndef VACUUMCLEANER_H
#define VACUUMCLEANER_H

#include "../include/House.h"
#include "../include/Sensor.h"
#include "../include/Algorithm.h"

#include <vector>
#include <string>
#include <utility>
#include <map>

class VacuumCleaner {
public:
    VacuumCleaner(House& house);

    void run();
    void run_with_sim();
    bool battery_exhausted() const;
    int get_total_steps_taken() const;
    string get_assignment_log() const;
    bool mission_completed(const House &house) const;

    // Sensors is a nested class inside VacuumCleaner
    class Sensors : public DirtSensor, public WallSensor, public BatterySensor {
    public:
        Sensors(const VacuumCleaner &vc) : vc(vc) {}

        // Sensor interface implementations
        int get_dirt_level() const override;

        map<string, bool> surrounding_walls() const override;

        int get_battery_level() const override;

    private:
        const VacuumCleaner &vc;
    };

private:
    House& house;
    int max_battery_steps;
    int max_steps_allowed;
    pair<int, int> docking_station_position;
    Algorithm algorithm;
    int total_steps_taken;
    int battery_level;
    int current_trip_steps;
    pair<int, int> position;
    vector<string> assignment_log;

    void move(string direction);
    void clean();
    pair<int, int> get_position() const;
    int get_current_trip_steps() const;
    void charge();
    void print_house(const string& action) const;
};

#endif // VACUUMCLEANER_H
