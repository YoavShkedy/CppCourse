#include "Algorithm_314939398.h"

#include <algorithm>  // for std::shuffle
#include <random>     // for std::default_random_engine
#include <chrono>     // for std::chrono::system_clock


Algorithm_314939398::Algorithm_314939398() : maxSteps(0), wallsSensor(nullptr), dirtSensor(nullptr), batteryMeter(nullptr),
                                             maxBatterySteps(0), totalSteps(0), dockingStation(0, 0), currPosition(0, 0),
                                             prevPosition(0, 0) {
    // Create a new Vertex for the docking station
    auto dockingStationVertex = std::make_shared<Vertex>(dockingStation, 0, Step::Stay);
    vertices[dockingStation] = dockingStationVertex;
    dirtyPos[dockingStation] = 0;
}

void Algorithm_314939398::setMaxSteps(std::size_t maxSteps) {
    this->maxSteps = maxSteps;
}

void Algorithm_314939398::setWallsSensor(const WallsSensor &wallsSensor) {
    this->wallsSensor = &wallsSensor;
}

void Algorithm_314939398::setDirtSensor(const DirtSensor &dirtSensor) {
    this->dirtSensor = &dirtSensor;
}

void Algorithm_314939398::setBatteryMeter(const BatteryMeter &batteryMeter) {
    this->batteryMeter = &batteryMeter;
}

Step Algorithm_314939398::nextStep() {
    Step res;
    if (firstStep) {
        firstStep = false;
        maxBatterySteps = batteryMeter->getBatteryState();
        relax();
    }
    if (totalSteps >= int(maxSteps)) {
        res = Step::Finish;
        return res;
    }
    if (returnToDockingStation) {
        if (currPosition == dockingStation) {
            // if on docking station AND battery not full -> charge
            if (batteryMeter->getBatteryState() < maxBatterySteps) {
                // Charge
                res = Step::Stay;
                totalSteps++;
                return res;
            } else { // Robot is charged. Navigate to the last position the robot was at
                returnToDockingStation = false;
                std::deque<Step> path = pathToLastPoint;
                // we need at least 2 * path.size() + 1 steps to be able to clean the last position
                if (2 * path.size() + 1 > maxSteps - totalSteps) {
                    res = Step::Finish;
                    return res;
                } else if (lastPosition != dockingStation) {
                    followPathToLastPoint = true;
                }
            }
        } else {
            relax();
            res = vertices[currPosition]->pai;
            pathToLastPoint.push_front(getOppositeStep(res));
            prevPosition = currPosition;
            updateCurrPosition(res);
            tripStepsLog.push_back(getOppositeStep(res));
            totalSteps++;
            return res;
        }
    }
    if (followPathToLastPoint && !pathToLastPoint.empty()) {
        res = pathToLastPoint.front();
        pathToLastPoint.erase(pathToLastPoint.begin());
        if (pathToLastPoint.empty()) {
            followPathToLastPoint = false;
        }
        prevPosition = currPosition;
        updateCurrPosition(res);
        totalSteps++;
        return res;
    }
    // relax only after position has changed
    if (currPosition != prevPosition) {
        relax();
    }
    if (int(batteryMeter->getBatteryState()) <= (vertices[currPosition]->d + 1) ||
        (int(maxSteps) - totalSteps) <= (vertices[currPosition]->d + 1)) {
        if (totalSteps == int(maxSteps)) {
            res = Step::Finish;
            return res;
        }
        returnToDockingStation = true;
        res = vertices[currPosition]->pai;
        lastPosition = currPosition;
        prevPosition = currPosition;
        updateCurrPosition(res);
        tripStepsLog.clear();
        tripStepsLog.push_back(getOppositeStep(res));
        totalSteps++;
        return res;
    }
    if (dirtSensor->dirtLevel() > 0) {
        vertices[currPosition]->vertexDirtLevel--;
        dirtyPos[currPosition] = dirtSensor->dirtLevel();
        res = Step::Stay;
        totalSteps++;
        return res;
    }
    res = chooseNeighbor();
    prevPosition = currPosition;
    updateCurrPosition(res);
    totalSteps++;
    dirtyPos[currPosition] = dirtSensor->dirtLevel();
    tripStepsLog.push_back(res);
    return res;
}

void Algorithm_314939398::updateCurrPosition(Step step) {
    currPosition = moveInDirection(currPosition, getMatchingDirection(step));
}

void Algorithm_314939398::relax() {
    auto currVertex = vertices[currPosition];
    currVertex->vertexDirtLevel = dirtSensor->dirtLevel();
    currVertex->visited = true;
    dirtyPos[currPosition] = dirtSensor->dirtLevel();
    for (Direction dir: {Direction::North, Direction::East, Direction::South, Direction::West}) {
        auto newPosition = moveInDirection(currVertex->position, dir);
        if (!wallsSensor->isWall(dir)) {
            if (vertices.find(newPosition) != vertices.end()) {
                // If newPosition exists in vertices
                if (currVertex->d > vertices[newPosition]->d + 1) {
                    currVertex->d = vertices[newPosition]->d + 1;
                    currVertex->pai = getMatchingStep(dir);
                }
                auto newVertex = vertices[newPosition];
                // Update neighbors list for both vertices
                if (std::find(currVertex->neighbors.begin(), currVertex->neighbors.end(), newPosition) ==
                    currVertex->neighbors.end()) {
                    currVertex->neighbors.push_back(newPosition);
                }
                if (std::find(newVertex->neighbors.begin(), newVertex->neighbors.end(), currVertex->position) ==
                    newVertex->neighbors.end()) {
                    newVertex->neighbors.push_back(currVertex->position);
                }
            } else {
                // newPosition does not exist in vertices
                auto newVertex = std::make_shared<Vertex>(newPosition, currVertex->d + 1,
                                                          getMatchingStep(getOppositeDirection(dir)));
                currVertex->neighbors.push_back(newPosition);
                newVertex->neighbors.push_back(currVertex->position);
                dirtyPos[newVertex->position] = newVertex->vertexDirtLevel; // newVertex->vertexDirtLevel = MAX_INT
                vertices[newPosition] = std::move(newVertex);
            }
        }
    }
}

Step Algorithm_314939398::chooseNeighbor() {
    std::vector<Direction> directions = {Direction::North, Direction::East, Direction::South, Direction::West};

    // Create a random number generator
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    // Shuffle the directions
    std::shuffle(directions.begin(), directions.end(), generator);

    Direction direction;
    int maxDirtLevel = -1;
    for (Direction dir: directions) {
        auto newPosition = moveInDirection(currPosition, dir);
        if (!wallsSensor->isWall(dir)) {
            if (vertices[newPosition]->vertexDirtLevel > maxDirtLevel) {
                maxDirtLevel = vertices[newPosition]->vertexDirtLevel;
                direction = dir;
            }
        }
    }
    return getMatchingStep(direction);
}

Step Algorithm_314939398::moveTo(std::pair<int, int> targetPosition) {
    if (currPosition.first < targetPosition.first) return Step::South;
    if (currPosition.first > targetPosition.first) return Step::North;
    if (currPosition.second < targetPosition.second) return Step::East;
    if (currPosition.second > targetPosition.second) return Step::West;
    return Step::Stay;
}

extern "C" {
REGISTER_ALGORITHM(Algorithm_314939398);
}
