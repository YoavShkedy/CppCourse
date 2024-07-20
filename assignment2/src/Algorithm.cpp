#include <iostream>
#include "../include/Algorithm.h"

Algorithm::Algorithm() : maxSteps(0), wallsSensor(nullptr), dirtSensor(nullptr), batteryMeter(nullptr), maxBatterySteps(0),
                         currTripSteps(0), totalSteps(0),
                         dockingStation(0, 0), currPosition(0, 0) {
    // Create a new Vertex for the docking station
    auto dockingStationVertex = std::make_shared<Vertex>(std::make_pair(0, 0), 0,
                                                         Step::Stay); // Step::Stay is used as null
    vertices[dockingStation] = std::move(dockingStationVertex);
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
    std::cout << "\nvertices[currPosition]->d: " << vertices[currPosition]->d << '\n' << std::endl;
    Step res;
    // the battery is guaranteed to be fully charged on the first step
    if (firstStep) {
        firstStep = false;
        maxBatterySteps = batteryMeter->getBatteryState();
        relax(); // relax() the docking station
    } // if no more steps allowed -> return 'Finish'
    if (totalSteps >= int(maxSteps)) {
        res = Step::Finish;
        return res;
    }
    /* on the way back to the docking station */
    if (returnToDockingStation) {
        std::cout << "\nreturnToDockingStation ==  TRUE" << std::endl;
        if (currPosition == dockingStation) {
            std::cout << "##### arrivedToDockingStation #####\n" << std::endl;
            // if battery is not full -> continue charging process
            if (batteryMeter->getBatteryState() < maxBatterySteps) {
                res = Step::Stay;
                totalSteps++;
                return res;
            } else { // on docking station with full battery
                std::cout << "\n<<<<< batteryFullyCharged >>>>>" << std::endl;
                std::cout << "returnToDockingStation ==  FALSE\n" << std::endl;
                returnToDockingStation = false;
                returnToLastCleaningPosition = true;
                std::cout << "\n$$$$$$$$$$$$$$$ END_PROCESS: returnToDockingStation $$$$$$$$$$$$$$$$$$$$$$$" << std::endl;
                std::cout << "\n$$$$$$$$$$$$$$$ START_PROCESS: returnToLastCleaningPosition $$$$$$$$$$$$$$$" << std::endl;
            }
        } else { // returnToDockingStation == True AND currPosition != dockingStation
            relax(); // relax() all the vertices on the way back to the docking station
            res = vertices[currPosition]->pai;
            updateCurrPosition(res);
            tripStepsLog.push_back(getOppositeStep(res));
            totalSteps++;
            return res;
        }
    }
    /* on the way back from the docking station to the last cleaning position */
    if (returnToLastCleaningPosition) {
        std::cout << "returnToLastCleaningPosition == TRUE\n" << std::endl;
        // if tripStepsLog is empty -> you returned to the last cleaning position, so continue with a new step
        if (tripStepsLog.empty()) {
            std::cout << "\n$$$$$$$$$$$$$$$ END_PROCESS: returnToLastCleaningPosition $$$$$$$$$$$$$$$" << std::endl;
            std::cout << "returnToLastCleaningPosition == FALSE" << std::endl;
            std::cout << "\nvertices[currPosition]->d: " << vertices[currPosition]->d << '\n' << std::endl;
            returnToLastCleaningPosition = false;
        } else { // continue returning to the last cleaning position
            relax(); // relax() all the vertices on the way back to the last cleaning position
            res = tripStepsLog.front();
            updateCurrPosition(res);
            tripStepsLog.pop_front();
            totalSteps++;
            return res;
        }
    }
    relax(); // get accurate distance from the docking station
    /* if battery OR available steps are low -> return to docking station */
    if (int(batteryMeter->getBatteryState()) <= (vertices[currPosition]->d + 1) ||
        (int(maxSteps) - totalSteps) <= (vertices[currPosition]->d + 1)) {
        // edge case: currPosition == dockingStation AND totalSteps == maxSteps
        if (totalSteps == int(maxSteps)) { // TODO: check if redundant
            res = Step::Finish;
            return res;
        }
        // start the process of returning to the docking station
        std::cout << "\n$$$$$$$$$$$$$$$ START_PROCESS: returnToDockingStation $$$$$$$$$$$$$$$" << std::endl;
        returnToDockingStation = true;
        // take the first step in the shortest path back to the docking station
        res = vertices[currPosition]->pai;
        updateCurrPosition(res);
        tripStepsLog.clear();
        tripStepsLog.push_back(getOppositeStep(res));
        totalSteps++;
        return res;
    }
    /* if the current position is dirty -> clean */
    if (dirtSensor->dirtLevel() > 0) {
        vertices[currPosition]->vertexDirtLevel--; // update currPosition dirt level
        res = Step::Stay;
        totalSteps++;
        return res;
    }
    /* choose the next step */
    res = chooseNeighbor();
    tripStepsLog.push_back(res); // record the step in the log
    updateCurrPosition(res);
    totalSteps++;
    return res;
}

void Algorithm::updateCurrPosition(Step step) {
    currPosition = moveInDirection(currPosition, getMatchingDirection(step));
}

void Algorithm::relax() {
    auto currVertex = vertices[currPosition];
    currVertex->vertexDirtLevel = dirtSensor->dirtLevel(); // update the currPosition dirt level
    currVertex->visited = true; // mark vertex as visited
    for (Direction dir: {Direction::North, Direction::East, Direction::South, Direction::West}) {
        auto newPosition = moveInDirection(currVertex->position, dir); // (currVertex->position) equals currPosition
        if (!wallsSensor->isWall(dir)) { // if newPosition != wall
            // if a vertex already exists for newPosition -> do relax
            if (vertices.find(newPosition) != vertices.end()) {
                if (currVertex->d > vertices[newPosition]->d + 1) {
                    currVertex->d = vertices[newPosition]->d + 1;
                    currVertex->pai = getMatchingStep(dir);
                }
            } else { // if currPosition does not have a vertex AND is not a wall -> create a new vertex
                auto newVertex = std::make_shared<Vertex>(newPosition, currVertex->d + 1,
                                                          getMatchingStep(getOppositeDirection(dir)));
                vertices[newPosition] = std::move(newVertex);
            }
        }
    }
}

Step Algorithm::chooseNeighbor() {
    Direction direction;
    int maxDirtLevel = -1;
    // all neighbors should already exist in vertices[] due to previous relax()
    for (Direction dir: {Direction::North, Direction::East, Direction::South, Direction::West}) {
        auto newPosition = moveInDirection(currPosition, dir); // currVertex->position == currPosition
        if (!wallsSensor->isWall(dir)) { // make sure newPosition is NOT a wall
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
    // Implement a pathfinding algorithm like A* to navigate back to the docking station
    // This is a placeholder for actual pathfinding logic
    return moveTo(targetPosition);
}

