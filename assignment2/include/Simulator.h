#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "utils.h"
#include "wall_sensor.h"
#include "dirt_sensor.h"
#include "battery_meter.h"
#include "Algorithm.h"


class Simulator : public WallsSensor, public DirtSensor, public BatteryMeter {
public:
    Simulator();
    void setAlgorithm(Algorithm algorithm);
    // WallSensor implementation
    bool isWall(Direction d) const override;
    // DirtSensor implementation
    int dirtLevel() const override;
    // BatteryMeter implementation
    std::size_t getBatteryState() const override;

    int getTotalDirt() const;

    std::pair<int, int> getSimCurrPosition();
    std::pair<int, int> getSimDockingStationPosition();

    void updateCurrentPosition(Step step);
    // Methods to set the environment
    void updateDirtLevel(int num);
    void updateBatteryLevel(float num);

    void readHouseFile(const std::string &filePath);
    void run();
    void runWithSim();
    void printHouseLayout() const;
    void printHouseLayoutForSim(const std::string& action) const;
    void updateSimTotalStepsLog(Step step);
    void createOutputFile();
    std::string parseSimTotalStepsLog();

private:
    int rows, cols, maxSteps;
    float maxBatterySteps = -1;
    std::string houseLayoutName;
    std::vector<std::vector<char>> houseLayout;
    bool dockingStationFound = false;
    std::pair<int, int> simDockingStationPosition;
    int totalDirt;
    int simTotalSteps;
    float batteryLevel;
    std::pair<int, int> simCurrPosition;
    Algorithm algo;
    std::vector<std::string> simTotalStepsLog;
    std::string input_file_name;
};

#endif // SIMULATOR_H
