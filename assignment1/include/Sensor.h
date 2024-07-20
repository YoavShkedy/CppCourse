#ifndef SENSOR_H
#define SENSOR_H

#include <map>
#include <tuple>

using namespace std;

class DirtSensor {
public:
    virtual int get_dirt_level() const = 0;
    virtual ~DirtSensor() = default;
};

class WallSensor {
public:
    virtual map<string, bool> surrounding_walls() const = 0;
    virtual ~WallSensor() = default;
};

class BatterySensor {
public:
    virtual int get_battery_level() const = 0;
    virtual ~BatterySensor() = default;
};

#endif // SENSOR_H
