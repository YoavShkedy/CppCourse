#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "wall_sensor.h"
#include "dirt_sensor.h"
#include "battery_meter.h"
#include "enums.h"
#include "Algorithm.h"
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <iterator>

class Simulator : public WallsSensor, public DirtSensor, public BatteryMeter {
public:
    Simulator();
    void setAlgorithm(Algorithm algo);
    // WallSensor implementation
    bool isWall(Direction d) const override;
    // DirtSensor implementation
    int dirtLevel() const override;
    // BatteryMeter implementation
    std::size_t getBatteryState() const override;

    int getTotalDirt();

    std::pair<int, int> getSimCurrPosition();
    std::pair<int, int> getSimDockingStationPosition();

    void updateCurrentPosition(Step step);
    // Methods to set the environment
    void updateDirtLevel(int num);
    void updateBatteryLevel(int num);

    void readHouseFile(const std::string &filePath);
    void run();
    void runWithSim();
    void printHouseLayout() const;
    void printHouseLayoutForSim(const std::string& action) const;
    void updateSimTotalStepsLog(Step step);

private:
    int rows, cols, maxSteps, maxBatterySteps = -1;
    std::string houseLayoutName;
    std::vector<std::vector<char>> houseLayout;
    bool dockingStationFound = false;
    std::pair<int, int> simDockingStationPosition;
    int totalDirt = 0;
    int simTotalSteps = 0;
    std::size_t batteryLevel;
    std::pair<int, int> simCurrPosition;
    Algorithm algo;
    std::vector<std::string> simTotalStepsLog;
};

#endif // SIMULATOR_H
