# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.30.2/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.30.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/build

# Include any dependencies generated for this target.
include CMakeFiles/myrobot.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/myrobot.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/myrobot.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/myrobot.dir/flags.make

CMakeFiles/myrobot.dir/simulator/main.cpp.o: CMakeFiles/myrobot.dir/flags.make
CMakeFiles/myrobot.dir/simulator/main.cpp.o: /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/simulator/main.cpp
CMakeFiles/myrobot.dir/simulator/main.cpp.o: CMakeFiles/myrobot.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/myrobot.dir/simulator/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/myrobot.dir/simulator/main.cpp.o -MF CMakeFiles/myrobot.dir/simulator/main.cpp.o.d -o CMakeFiles/myrobot.dir/simulator/main.cpp.o -c /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/simulator/main.cpp

CMakeFiles/myrobot.dir/simulator/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/myrobot.dir/simulator/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/simulator/main.cpp > CMakeFiles/myrobot.dir/simulator/main.cpp.i

CMakeFiles/myrobot.dir/simulator/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/myrobot.dir/simulator/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/simulator/main.cpp -o CMakeFiles/myrobot.dir/simulator/main.cpp.s

CMakeFiles/myrobot.dir/simulator/Simulator.cpp.o: CMakeFiles/myrobot.dir/flags.make
CMakeFiles/myrobot.dir/simulator/Simulator.cpp.o: /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/simulator/Simulator.cpp
CMakeFiles/myrobot.dir/simulator/Simulator.cpp.o: CMakeFiles/myrobot.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/myrobot.dir/simulator/Simulator.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/myrobot.dir/simulator/Simulator.cpp.o -MF CMakeFiles/myrobot.dir/simulator/Simulator.cpp.o.d -o CMakeFiles/myrobot.dir/simulator/Simulator.cpp.o -c /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/simulator/Simulator.cpp

CMakeFiles/myrobot.dir/simulator/Simulator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/myrobot.dir/simulator/Simulator.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/simulator/Simulator.cpp > CMakeFiles/myrobot.dir/simulator/Simulator.cpp.i

CMakeFiles/myrobot.dir/simulator/Simulator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/myrobot.dir/simulator/Simulator.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/simulator/Simulator.cpp -o CMakeFiles/myrobot.dir/simulator/Simulator.cpp.s

CMakeFiles/myrobot.dir/simulator/AlgorithmRegistrar.cpp.o: CMakeFiles/myrobot.dir/flags.make
CMakeFiles/myrobot.dir/simulator/AlgorithmRegistrar.cpp.o: /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/simulator/AlgorithmRegistrar.cpp
CMakeFiles/myrobot.dir/simulator/AlgorithmRegistrar.cpp.o: CMakeFiles/myrobot.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/myrobot.dir/simulator/AlgorithmRegistrar.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/myrobot.dir/simulator/AlgorithmRegistrar.cpp.o -MF CMakeFiles/myrobot.dir/simulator/AlgorithmRegistrar.cpp.o.d -o CMakeFiles/myrobot.dir/simulator/AlgorithmRegistrar.cpp.o -c /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/simulator/AlgorithmRegistrar.cpp

CMakeFiles/myrobot.dir/simulator/AlgorithmRegistrar.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/myrobot.dir/simulator/AlgorithmRegistrar.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/simulator/AlgorithmRegistrar.cpp > CMakeFiles/myrobot.dir/simulator/AlgorithmRegistrar.cpp.i

CMakeFiles/myrobot.dir/simulator/AlgorithmRegistrar.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/myrobot.dir/simulator/AlgorithmRegistrar.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/simulator/AlgorithmRegistrar.cpp -o CMakeFiles/myrobot.dir/simulator/AlgorithmRegistrar.cpp.s

CMakeFiles/myrobot.dir/utils/utils.cpp.o: CMakeFiles/myrobot.dir/flags.make
CMakeFiles/myrobot.dir/utils/utils.cpp.o: /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/utils/utils.cpp
CMakeFiles/myrobot.dir/utils/utils.cpp.o: CMakeFiles/myrobot.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/myrobot.dir/utils/utils.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/myrobot.dir/utils/utils.cpp.o -MF CMakeFiles/myrobot.dir/utils/utils.cpp.o.d -o CMakeFiles/myrobot.dir/utils/utils.cpp.o -c /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/utils/utils.cpp

CMakeFiles/myrobot.dir/utils/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/myrobot.dir/utils/utils.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/utils/utils.cpp > CMakeFiles/myrobot.dir/utils/utils.cpp.i

CMakeFiles/myrobot.dir/utils/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/myrobot.dir/utils/utils.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/utils/utils.cpp -o CMakeFiles/myrobot.dir/utils/utils.cpp.s

# Object files for target myrobot
myrobot_OBJECTS = \
"CMakeFiles/myrobot.dir/simulator/main.cpp.o" \
"CMakeFiles/myrobot.dir/simulator/Simulator.cpp.o" \
"CMakeFiles/myrobot.dir/simulator/AlgorithmRegistrar.cpp.o" \
"CMakeFiles/myrobot.dir/utils/utils.cpp.o"

# External object files for target myrobot
myrobot_EXTERNAL_OBJECTS =

myrobot: CMakeFiles/myrobot.dir/simulator/main.cpp.o
myrobot: CMakeFiles/myrobot.dir/simulator/Simulator.cpp.o
myrobot: CMakeFiles/myrobot.dir/simulator/AlgorithmRegistrar.cpp.o
myrobot: CMakeFiles/myrobot.dir/utils/utils.cpp.o
myrobot: CMakeFiles/myrobot.dir/build.make
myrobot: Algorithm_206448649_314939398_A/libAlgorithm_206448649_314939398_A.dylib
myrobot: Algorithm_206448649_314939398_B/libAlgorithm_206448649_314939398_B.dylib
myrobot: CMakeFiles/myrobot.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable myrobot"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/myrobot.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/myrobot.dir/build: myrobot
.PHONY : CMakeFiles/myrobot.dir/build

CMakeFiles/myrobot.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/myrobot.dir/cmake_clean.cmake
.PHONY : CMakeFiles/myrobot.dir/clean

CMakeFiles/myrobot.dir/depend:
	cd /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/build /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/build /Users/benshushan/Desktop/TAU/ThirdYear/C++/FinalProject/assignment3/src/build/CMakeFiles/myrobot.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/myrobot.dir/depend

