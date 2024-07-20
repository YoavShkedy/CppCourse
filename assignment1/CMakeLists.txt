cmake_minimum_required(VERSION 3.23)
project(Cpp_Course)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(include)

add_executable(Cpp_Course
        include/Algorithm.h
        include/House.h
        include/InputGenerator.h
        include/Sensor.h
        include/VacuumCleaner.h
        src/Algorithm.cpp
        src/House.cpp
        src/InputGenerator.cpp
        src/main.cpp
        src/VacuumCleaner.cpp)

# GoogleTest setup
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Add test executable and link with GoogleTest and project files
add_executable(runTests
        tests/HouseTest.cpp
        tests/VacuumCleanerTest.cpp
        tests/AlgorithmTest.cpp
        src/Algorithm.cpp
        src/House.cpp
        src/InputGenerator.cpp
        src/VacuumCleaner.cpp)

target_link_libraries(runTests gtest gtest_main gmock gmock_main)
add_test(NAME runTests COMMAND runTests)
