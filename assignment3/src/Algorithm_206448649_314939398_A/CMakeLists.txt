cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_SHARED_LINKER_FLAGS "-fPIC -shared -fvisibility=hidden -fvisibility-inlines-hidden")

project(Algorithm_206448649_314939398_A)

# Set the include directories
set(INCLUDES    ${PROJECT_SOURCE_DIR}/../algorithm/
        ${PROJECT_SOURCE_DIR}/../common/
        ${PROJECT_SOURCE_DIR}/../Algorithm_206448649_314939398_A)

# Set the source files
set(SOURCES     ${PROJECT_SOURCE_DIR}/Algorithm_206448649_314939398_A.cpp)

# Create a shared library target
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Specify the include directories for the target
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})

# Set the output directory for the .so file
set(ALGO_LIB_DIR ${CMAKE_BINARY_DIR}/../build)

# Create the output directory if it doesn't exist
file(MAKE_DIRECTORY ${ALGO_LIB_DIR})

# Copy the .so file to the output directory after building
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${ALGO_LIB_DIR}
        )
