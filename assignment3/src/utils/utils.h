#ifndef ASSIGNMENT3_UTILS_H
#define ASSIGNMENT3_UTILS_H

#include "enums.h"
#include <queue>
#include <unordered_map>
#include <memory>
#include <thread>
#include <vector>
#include <utility> // For std::pair
#include <limits>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <string>
#include <cmath>
#include <boost/functional/hash.hpp> // For boost::hash
#include <mutex>
#include <dlfcn.h>


// Utility function declarations
Direction getOppositeDirection(Direction dir);

Step getOppositeStep(Step step);

Step getMatchingStep(Direction dir);

Direction getMatchingDirection(Step step);

std::string getMatchingString(Step step);

std::pair<int, int> moveInDirection(std::pair<int, int> position, Direction dir);

class Vertex {
public:
    std::pair<int, int> position;
    bool visited;
    int d; // The distance in steps from the docking station
    Step pai; // The step need to take for the shortest path back to the docking station
    int vertexDirtLevel = INT_MAX;
    std::vector<std::pair<int, int>> neighbors;

    Vertex(std::pair<int, int> position, int d, Step pai) : position(position), visited(false), d(d), pai(pai) {}

    bool operator==(const Vertex &other) const {
        return position == other.position;
    }
};

// Custom hash function for Vertex
struct VertexHash {
    size_t operator()(const std::pair<int, int> &position) const {
        return boost::hash<std::pair<int, int>>()(position);
    }
};

#endif //ASSIGNMENT3_UTILS_H
