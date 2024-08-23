cmake_minimum_required(VERSION 3.16)
project(myrobot)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# Set linker flags if necessary
SET(CMAKE_EXE_LINKER_FLAGS "-rdynamic")

# Find Boost
find_package(Boost REQUIRED)

# Add subdirectories for the algorithms
add_subdirectory(Algorithm_206448649_314939398_A)
add_subdirectory(Algorithm_206448649_314939398_B)

# Set paths for common directories
set(SIM_DIR ${PROJECT_SOURCE_DIR}/simulator)
set(COMMON_DIR ${PROJECT_SOURCE_DIR}/common)
set(UTILS_DIR ${PROJECT_SOURCE_DIR}/utils)
set(ALGO_DIR ${PROJECT_SOURCE_DIR}/algorithm)

# Create the executable target
add_executable(myrobot
        ${SIM_DIR}/main.cpp
        ${SIM_DIR}/Simulator.cpp
        ${SIM_DIR}/AlgorithmRegistrar.cpp
        ${UTILS_DIR}/utils.cpp
        )

# Set C++20 standard specifically for the myrobot target
target_compile_features(myrobot PRIVATE cxx_std_20)
set_property(TARGET myrobot PROPERTY CXX_STANDARD 20)

# If using GCC or Clang, explicitly enable C++20 features
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(myrobot PRIVATE -std=c++20)
endif()

# Include directories specific to the target
target_include_directories(myrobot PRIVATE
        ${SIM_DIR}
        ${COMMON_DIR}
        ${UTILS_DIR}
        ${ALGO_DIR}
        ${Boost_INCLUDE_DIRS}
        )

# Link libraries specific to the target
target_link_libraries(myrobot PRIVATE
        Algorithm_206448649_314939398_A
        Algorithm_206448649_314939398_B
        ${Boost_LIBRARIES}
        dl
        pthread
        )
