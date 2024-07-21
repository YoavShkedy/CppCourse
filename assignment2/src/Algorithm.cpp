#include <iostream>
#include <queue>
#include "../include/Algorithm.h"
#include "../include/utils.h"
#include <thread>
#include <chrono>

Algorithm::Algorithm() : maxSteps(0), wallsSensor(nullptr), dirtSensor(nullptr), batteryMeter(nullptr), maxBatterySteps(0),
                         currTripSteps(0), totalSteps(0),
                         dockingStation(0, 0), currPosition(0, 0) {
    // Create a new Vertex for the docking station
    auto dockingStationVertex = std::make_shared<Vertex>(dockingStation, 0, Step::Stay);
    vertices[dockingStation] = dockingStationVertex;
    dirtyPos[dockingStation] = 0;
}

void Algorithm::setMaxSteps(std::size_t maxSteps) {
    this->maxSteps = maxSteps;
}

void Algorithm::setWallsSensor(const WallsSensor &wallsSensor) {
    this->wallsSensor = &wallsSensor;
}

void Algorithm::setDirtSensor(const DirtSensor &dirtSensor) {
    this->dirtSensor = &dirtSensor;
}

void Algorithm::setBatteryMeter(const BatteryMeter &batteryMeter) {
    this->batteryMeter = &batteryMeter;
}

Step Algorithm::nextStep() {
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
            if (batteryMeter->getBatteryState() < maxBatterySteps) {
                // Charge
                res = Step::Stay;
                totalSteps++;
                return res;
            } else {
                // Robot is charged. Navigate to the closest dirty position
                returnToDockingStation = false;
                std::vector<Step> path;
                auto closestDirtyPoint = findClosestDirtyPoint(path);
                if (maxSteps - totalSteps < 2 * path.size()) {
                    res = Step::Finish;
                    return res;
                }
                else if (closestDirtyPoint != dockingStation) {
                    pathToDirtyPoint = path;
                    followPathToDirtyPoint = true;
                }
            }
        } else {
            relax();
            res = vertices[currPosition]->pai;
            updateCurrPosition(res);
            tripStepsLog.push_back(getOppositeStep(res));
            totalSteps++;
            return res;
        }
    }
    if (followPathToDirtyPoint && !pathToDirtyPoint.empty()) {
        res = pathToDirtyPoint.front();
        pathToDirtyPoint.erase(pathToDirtyPoint.begin());
        if (pathToDirtyPoint.empty()) {
            followPathToDirtyPoint = false;
        }
        updateCurrPosition(res);
        totalSteps++;
        return res;
    }
    relax();
    if (int(batteryMeter->getBatteryState()) <= (vertices[currPosition]->d + 1) ||
        (int(maxSteps) - totalSteps) <= (vertices[currPosition]->d + 1)) {
        if (totalSteps == int(maxSteps)) {
            res = Step::Finish;
            return res;
        }
        returnToDockingStation = true;
        res = vertices[currPosition]->pai;
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
    updateCurrPosition(res);
    totalSteps++;
    dirtyPos[currPosition] = dirtSensor->dirtLevel();
    tripStepsLog.push_back(res);
    return res;
}

void Algorithm::updateCurrPosition(Step step) {
    currPosition = moveInDirection(currPosition, getMatchingDirection(step));
}

void Algorithm::relax() {
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
                if (std::find(currVertex->neighbors.begin(), currVertex->neighbors.end(), newPosition) == currVertex->neighbors.end()) {
                    currVertex->neighbors.push_back(newPosition);
                }
                if (std::find(newVertex->neighbors.begin(), newVertex->neighbors.end(), currVertex->position) == newVertex->neighbors.end()) {
                    newVertex->neighbors.push_back(currVertex->position);
                }
            } else {
                // newPosition does not exist in vertices
                auto newVertex = std::make_shared<Vertex>(newPosition, currVertex->d + 1, getMatchingStep(getOppositeDirection(dir)));
                currVertex->neighbors.push_back(newPosition);
                newVertex->neighbors.push_back(currVertex->position);
                dirtyPos[newVertex->position] = newVertex->vertexDirtLevel; // newVertex->vertexDirtLevel = MAX_INT
                vertices[newPosition] = std::move(newVertex);
            }
        }
    }
}

Step Algorithm::chooseNeighbor() {
    Direction direction;
    int maxDirtLevel = -1;
    for (Direction dir: {Direction::North, Direction::East, Direction::South, Direction::West}) {
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

Step Algorithm::moveTo(std::pair<int, int> targetPosition) {
    if (currPosition.first < targetPosition.first) return Step::South;
    if (currPosition.first > targetPosition.first) return Step::North;
    if (currPosition.second < targetPosition.second) return Step::East;
    if (currPosition.second > targetPosition.second) return Step::West;
    return Step::Stay;
}

Step Algorithm::navigateTo(std::pair<int, int> targetPosition) {
    return moveTo(targetPosition);
}

std::pair<int, int> Algorithm::findClosestDirtyPoint(std::vector<Step>& path) {
    std::queue<std::pair<int, int>> q;
    std::unordered_map<std::pair<int, int>, bool, VertexHash> visited;
    std::unordered_map<std::pair<int, int>, std::pair<int, int>, VertexHash> parent;
    std::unordered_map<std::pair<int, int>, int, VertexHash> distance;

    q.push(dockingStation);
    visited[dockingStation] = true;
    distance[dockingStation] = 0;

    std::pair<int, int> closestDirtyPoint = dockingStation;
    int maxDirtLevel = 0;
    bool foundClosest = false;

    while (!q.empty() && !foundClosest) {
        int size = q.size();  // Number of nodes at the current distance level

        for (int i = 0; i < size; ++i) {
            auto current = q.front();
            q.pop();

            if (dirtyPos[current] > maxDirtLevel) {
                closestDirtyPoint = current;
                maxDirtLevel = dirtyPos[current];
                foundClosest = true;
            }

            for (const auto& neighbor : vertices[current]->neighbors) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    parent[neighbor] = current;
                    distance[neighbor] = distance[current] + 1;
                    q.push(neighbor);
                }
            }
        }
    }

    std::pair<int, int> step = closestDirtyPoint;
    while (step != dockingStation) {
        auto parentStep = parent[step];
        path.insert(path.begin(), moveTo(step));
        step = parentStep;
    }

    return closestDirtyPoint;
}
