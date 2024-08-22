cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_SHARED_LINKER_FLAGS "-fPIC -shared -fvisibility=hidden -fvisibility-inlines-hidden")

project(Algorithm_206448649_314939398_B)

# Set paths for common directories
set(COMMON_DIR ${PROJECT_SOURCE_DIR}/../common)
set(UTILS_DIR ${PROJECT_SOURCE_DIR}/../utils)
set(ALGO_DIR ${PROJECT_SOURCE_DIR}/../algorithm)

# Include directories
set(INCLUDES
        ${COMMON_DIR}
        ${UTILS_DIR}
        ${ALGO_DIR}
        )

set(SOURCES Algorithm_206448649_314939398_B.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})

set(ALGO_LIB_DIR ${CMAKE_BINARY_DIR}/algo_dir)

file(MAKE_DIRECTORY ${ALGO_LIB_DIR})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${ALGO_LIB_DIR})
