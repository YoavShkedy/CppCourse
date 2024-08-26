#!/bin/bash

# Navigate to the source directory
cd /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/

# Remove all '.csv' and '.error' files
find . -name "*.csv" -type f -delete
find . -name "*.error" -type f -delete

# Remove the build directory if it exists
rm -rf build

# Create the build directory
#mkdir build

# Configure and build the project using the new CMake commands
#cmake -S . -B build
#cmake --build build
