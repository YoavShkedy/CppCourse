#include "../include/utils.h"

Direction getOppositeDirection(Direction dir) {
    switch (dir) {
        case Direction::North: return Direction::South;
        case Direction::East: return Direction::West;
        case Direction::South: return Direction::North;
        case Direction::West: return Direction::East;
    }
    return Direction::North; // Default to North to avoid compiler warning
}

Step getOppositeStep(Step step) {
    switch (step) {
        case Step::North: return Step::South;
        case Step::East: return Step::West;
        case Step::South: return Step::North;
        case Step::West: return Step::East;
        default: return Step::Stay;
    }
}

Step getMatchingStep(Direction dir) {
    switch (dir) {
        case Direction::North: return Step::North;
        case Direction::East: return Step::East;
        case Direction::South: return Step::South;
        case Direction::West: return Step::West;
    }
}

Direction getMatchingDirection(Step step) {
    switch (step) {
        case Step::North: return Direction::North;
        case Step::East: return Direction::East;
        case Step::South: return Direction::South;
        case Step::West: return Direction::West;
        default: return Direction::North; // Default to North to avoid compiler warning
    }
}

std::string getMatchingString(Step step) {
    switch (step) {
        case Step::North: return "North";
        case Step::East: return "East";
        case Step::South: return "South";
        case Step::West: return "West";
        case Step::Stay: return "Stay";
        case Step::Finish: return "Finish";
    }
}

std::pair<int, int> moveInDirection(std::pair<int, int> position, Direction dir) {
    switch (dir) {
        case Direction::North: return {position.first - 1, position.second};
        case Direction::East: return {position.first, position.second + 1};
        case Direction::South: return {position.first + 1, position.second};
        case Direction::West: return {position.first, position.second - 1};
    }
    return position; // Default to no move to avoid compiler warning
}
