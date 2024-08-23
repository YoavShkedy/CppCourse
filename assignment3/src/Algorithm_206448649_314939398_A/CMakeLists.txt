cmake_minimum_required(VERSION 3.16)

project(Algorithm_206448649_314939398_A)

SET(CMAKE_SHARED_LINKER_FLAGS "-fPIC -shared -fvisibility=hidden -fvisibility-inlines-hidden")

set(INCLUDES ${PROJECT_SOURCE_DIR}/../algorithm/
        ${PROJECT_SOURCE_DIR}/../common/
        ${PROJECT_SOURCE_DIR}/../utils/)

set(SOURCES Algorithm_206448649_314939398_A.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})

set(ALGO_LIB_DIR ${CMAKE_BINARY_DIR}/algo_dir)

file(MAKE_DIRECTORY ${ALGO_LIB_DIR})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${ALGO_LIB_DIR}
        )

