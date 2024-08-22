cmake_minimum_required(VERSION 3.16)

project(myrobot)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# Set linker flags if necessary
SET(CMAKE_EXE_LINKER_FLAGS "-rdynamic")

# Specify the Boost root directory
set(BOOST_ROOT /opt/homebrew/Cellar/boost/1.86.0)

# Find Boost
find_package(Boost REQUIRED)

# Set paths for common directories
set(SIM_DIR simulator)
set(COMMON_DIR common)
set(UTILS_DIR utils)
set(ALGO_DIR algorithm)

# Create the executable target
add_executable(myrobot
        ${SIM_DIR}/main.cpp  # Ensure main.cpp is included
        ${SIM_DIR}/Simulator.cpp
        ${SIM_DIR}/AlgorithmRegistrar.cpp
        )

# Include directories specific to the target
target_include_directories(myrobot PRIVATE
        ${SIM_DIR}
        ${COMMON_DIR}
        ${UTILS_DIR}   # Added for utils.h
        ${ALGO_DIR}    # Added for AlgorithmRegistration.h
        ${Boost_INCLUDE_DIRS}  # Boost include directories
        )

# Link libraries specific to the target
