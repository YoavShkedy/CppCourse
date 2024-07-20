#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "utils.h"
#include "abstract_algorithm.h"
#include <queue>
#include <unordered_map>
#include <memory>

class Algorithm : public AbstractAlgorithm {
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
    std::pair<int, int> prevPosition;
    bool returnToDockingStation = false;
    bool returnToLastCleaningPosition = false;


public:
    Algorithm();

    void setMaxSteps(size_t maxSteps) override;
    void setWallsSensor(const WallsSensor& wallsSensor) override;
    void setDirtSensor(const DirtSensor& dirtSensor) override;
    void setBatteryMeter(const BatteryMeter& batteryMeter) override;
    Step nextStep() override;

private:
    Step moveTo(std::pair<int, int> targetPosition);
    void updateCurrPosition(Step step);
    void relax();
    Step chooseNeighbor();
    Step navigateTo(std::pair<int, int> targetPosition);
};

#endif // ALGORITHM_H
