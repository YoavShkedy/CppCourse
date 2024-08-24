cmake_minimum_required(VERSION 3.16)

project(Algorithm_206448649_314939398_A)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# Set linker flags
SET(CMAKE_SHARED_LINKER_FLAGS "-fPIC -shared -fvisibility=hidden -fvisibility-inlines-hidden")

# Suppress the warning for FindBoost module
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 OLD)
endif()

find_package(Boost REQUIRED)

include_directories(
        ${Boost_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/../algorithm/
        ${PROJECT_SOURCE_DIR}/../common/
        ${PROJECT_SOURCE_DIR}/../utils/
)

set(SOURCES Algorithm_206448649_314939398_A.cpp
        ${PROJECT_SOURCE_DIR}/../utils/utils.cpp
        ${PROJECT_SOURCE_DIR}/../simulator/AlgorithmRegistrar.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

set(ALGO_LIB_DIR ${CMAKE_BINARY_DIR}/algo_dir)

file(MAKE_DIRECTORY ${ALGO_LIB_DIR})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${ALGO_LIB_DIR}
        )
