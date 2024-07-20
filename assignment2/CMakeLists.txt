cmake_minimum_required(VERSION 3.23)
project(myrobot_project)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set compiler flags
add_compile_options(-Wall -Wextra -Werror -pedantic)

# Include directories
include_directories(include)

# Find Boost
find_package(Boost REQUIRED)

# Include Boost directories
include_directories(${Boost_INCLUDE_DIRS})

# Add the executable and source files
add_executable(myrobot
        src/main.cpp
        src/utils.cpp
        src/Algorithm.cpp
        src/Simulator.cpp
        )

# Set include directories for the executable
target_include_directories(myrobot PRIVATE include)
