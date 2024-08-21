cmake_minimum_required(VERSION 3.23)
project(assignment3)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

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
        src/Algorithm_206448649_314939398_A/Algorithm_206448649_314939398_A.cpp
        src/Algorithm_206448649_314939398_B/Algorithm_206448649_314939398_B.cpp
        src/utils/utils.cpp
        )
