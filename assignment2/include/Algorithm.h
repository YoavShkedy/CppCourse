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
    int totalSteps;
    std::deque<Step> tripStepsLog;
    std::unordered_map<std::pair<int, int>, std::shared_ptr<Vertex>, VertexHash> vertices;
    std::unordered_map<std::pair<int, int>, int, VertexHash> dirtyPos;
    std::pair<int, int> dockingStation;
    std::pair<int, int> currPosition;
    std::pair<int, int> prevPosition;
    bool returnToDockingStation = false;
    bool followPathToDirtyPoint = false;
    std::vector<Step> pathToDirtyPoint;

    // private methods:
    Step moveTo(std::pair<int, int> targetPosition);
    void updateCurrPosition(Step step);
    void relax();
    Step chooseNeighbor();
    std::pair<int, int> findClosestDirtyPoint(std::vector<Step>& path);
};

#endif // ALGORITHM_H
