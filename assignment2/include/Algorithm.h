#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "utils.h"
#include "abstract_algorithm.h"


class Algorithm : public AbstractAlgorithm {
public:
    Algorithm();

    void setMaxSteps(size_t maxSteps) override;
    void setWallsSensor(const WallsSensor& wallsSensor) override;
    void setDirtSensor(const DirtSensor& dirtSensor) override;
    void setBatteryMeter(const BatteryMeter& batteryMeter) override;
    Step nextStep() override;

private:
    size_t maxSteps;
    const WallsSensor* wallsSensor;
    const DirtSensor* dirtSensor;
    const BatteryMeter* batteryMeter;
    size_t maxBatterySteps;
    bool firstStep = true;
    [[maybe_unused]] int currTripSteps;
    int totalSteps;
    std::deque<Step> tripStepsLog;
    std::queue<std::shared_ptr<Vertex>> bfsQueue;
    std::unordered_map<std::pair<int, int>, std::shared_ptr<Vertex>, VertexHash> vertices;
    std::pair<int, int> dockingStation;
    std::pair<int, int> currPosition;
    bool returnToDockingStation = false;
    [[maybe_unused]] bool returnToLastCleaningPosition = false;
    bool followPathToDirtyPoint = false;
    std::vector<Step> pathToDirtyPoint;

    // private functions:
    Step moveTo(std::pair<int, int> targetPosition);
    void updateCurrPosition(Step step);
    void relax();
    Step chooseNeighbor();
    Step navigateTo(std::pair<int, int> targetPosition);
    std::pair<int, int> findClosestDirtyPoint(std::vector<Step>& path);
};

#endif // ALGORITHM_H
