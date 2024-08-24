#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "utils.h"
#include "../Algorithm_206448649_314939398_A/Algorithm_206448649_314939398_A.h"
#include "../Algorithm_206448649_314939398_B/Algorithm_206448649_314939398_B.h"
#include "WallSensor.h"
#include "DirtSensor.h"
#include "BatteryMeter.h"


class Simulator : public WallsSensor, public DirtSensor, public BatteryMeter {
public:
    Simulator();

    explicit Simulator(bool summaryOnly);

    void setAlgorithm(std::unique_ptr<AbstractAlgorithm> algorithm);

    // WallSensor implementation
    bool isWall(Direction d) const override;

    // DirtSensor implementation
    int dirtLevel() const override;

    // BatteryMeter implementation
    std::size_t getBatteryState() const override;

    bool readHouseFile(const std::string &filePath);

    int run();

    void runWithSim();


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
    std::unique_ptr<AbstractAlgorithm> algo;
    std::string algoName;
    std::vector<std::string> simTotalStepsLog;
    std::string input_file_name;
    bool summaryOnly = false;


    // private methods:
    std::pair<int, int> getSimCurrPosition();

    std::pair<int, int> getSimDockingStationPosition();

    void updateCurrentPosition(Step step);

    void updateDirtLevel(int num);

    void updateBatteryLevel(float num);

    void setBatteryLevel(float num);

    int getTotalDirt() const;

    void printHouseLayout() const;

    void printHouseLayoutForSim(const std::string &action) const;

    void updateSimTotalStepsLog(Step step);

    void createOutputFile();

    std::string parseSimTotalStepsLog();

    std::string getAlgorithmName(const std::unique_ptr<AbstractAlgorithm> &algorithm);

    std::string calcStatus();

    std::string calcInDock();

    int calcScore();
};

#endif // SIMULATOR_H
