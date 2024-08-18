cmake_minimum_required(VERSION 3.23)
project(assignment3)

set(CMAKE_CXX_STANDARD 14)

include_directories(algorithm)
include_directories(common)

add_executable(assignment3
        algorithm/Algorithm_a.cpp
        algorithm/Algorithm_a.h
        algorithm/AlgorithmRegistration.h
        common/AbstractAlgorithm.h
        common/AlgorithmRegistrar.h
        common/BatteryMeter.h
        common/DirtSensor.h
        common/enums.h
        common/WallSensor.h
        simulator/AlgorithmRegistrar.cpp)
