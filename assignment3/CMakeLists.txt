cmake_minimum_required(VERSION 3.16)

project(my_robot)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# Set linker flags if necessary
SET(CMAKE_EXE_LINKER_FLAGS "-rdynamic")

# Include subdirectories for algorithms
add_subdirectory(Algorithm_206448649_314939398_A)
add_subdirectory(Algorithm_206448649_314939398_B)

# Set paths for common directories
set(SIM_DIR ${PROJECT_SOURCE_DIR}/simulator)
set(COMMON_DIR ${PROJECT_SOURCE_DIR}/common)

# Create the executable
add_executable(${PROJECT_NAME}
        main.cpp
        ${SIM_DIR}/Simulator.cpp
        ${SIM_DIR}/AlgorithmRegistrar.cpp
        )

# Include directories specific to the target
target_include_directories(${PROJECT_NAME} PRIVATE
        ${SIM_DIR}
        ${COMMON_DIR}
        )

# Link libraries specific to the target
target_link_libraries(${PROJECT_NAME} PRIVATE
        Algorithm_206448649_314939398_A
        Algorithm_206448649_314939398_B
        dl
        pthread
        )

# Set compiler flags
add_compile_options(-Wall -Wextra -Werror -pedantic)

# Find Boost and link it to the target
find_package(Boost REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
