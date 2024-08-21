cmake_minimum_required(VERSION 3.23)
project(assignment3)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 14)

# Include directories
include_directories(src/common)
include_directories(src/algorithm)
include_directories(src/utils)

# Set compiler flags
add_compile_options(-Wall -Wextra -Werror -pedantic)

# Find Boost
find_package(Boost REQUIRED)
# Include Boost directories
include_directories(${Boost_INCLUDE_DIRS})

# Add executable
add_executable(myrobot
        src/simulator/main.cpp
        src/simulator/Simulator.cpp
        src/simulator/AlgorithmRegistrar.cpp
        src/Algorithm_206448649/Algorithm_206448649.cpp
        src/Algorithm_314939398/Algorithm_314939398.cpp
        src/utils/utils.cpp
        )
