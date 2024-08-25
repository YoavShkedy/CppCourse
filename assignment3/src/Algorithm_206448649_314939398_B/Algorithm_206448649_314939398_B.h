#ifndef ASSIGNMENT3_ALGORITHM_206448649_314939398_B_H
#define ASSIGNMENT3_ALGORITHM_206448649_314939398_B_H

#include "utils.h"
#include "enums.h"
#include "DirtSensor.h"
#include "WallSensor.h"
#include "BatteryMeter.h"
#include "AbstractAlgorithm.h"
#include "AlgorithmRegistrar.h"
#include "AlgorithmRegistration.h"

#include <deque>

class Algorithm_206448649_314939398_B : public AbstractAlgorithm {
public:
    Algorithm_206448649_314939398_B();

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
    int totalSteps;
    std::deque<Step> tripStepsLog;
    std::unordered_map<std::pair<int, int>, std::shared_ptr<Vertex>, VertexHash> vertices;
    std::unordered_map<std::pair<int, int>, int, VertexHash> dirtyPos;
    std::pair<int, int> dockingStation;
    std::pair<int, int> currPosition;
    std::pair<int, int> prevPosition;
    std::pair<int, int> lastPosition;
    bool returnToDockingStation = false;
    bool followPathToLastPoint = false;
    std::deque<Step> pathToLastPoint;

    // private methods:
    Step moveTo(std::pair<int, int> targetPosition);
    void updateCurrPosition(Step step);
    void relax();
    Step chooseNeighbor();
};



#endif //ASSIGNMENT3_ALGORITHM_206448649_314939398_B_H