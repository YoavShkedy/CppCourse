#ifndef UTILS_H
#define UTILS_H

#include "enums.h"
#include <utility> // For std::pair
#include <boost/functional/hash.hpp> // For boost::hash
#include <limits>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>


// Define logging macros
#define LOG_DEBUG(...) spdlog::debug(__VA_ARGS__)
#define LOG_INFO(...) spdlog::info(__VA_ARGS__)
#define LOG_WARN(...) spdlog::warn(__VA_ARGS__)
#define LOG_ERROR(...) spdlog::error(__VA_ARGS__)

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

    bool operator==(const Vertex& other) const {
        return position == other.position;
    }
};

// Custom hash function for Vertex
struct VertexHash {
    size_t operator()(const std::pair<int, int>& position) const {
        return boost::hash<std::pair<int, int>>()(position);
    }
};

#endif // UTILS_H
