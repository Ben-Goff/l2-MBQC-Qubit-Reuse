# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.26.4/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.26.4/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/ben/github/l2-MBQC-Qubit-Reuse

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ben/github/l2-MBQC-Qubit-Reuse/build

# Include any dependencies generated for this target.
include CMakeFiles/circuit.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/circuit.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/circuit.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/circuit.dir/flags.make

CMakeFiles/circuit.dir/src/qcircuit.cxx.o: CMakeFiles/circuit.dir/flags.make
CMakeFiles/circuit.dir/src/qcircuit.cxx.o: /Users/ben/github/l2-MBQC-Qubit-Reuse/src/qcircuit.cxx
CMakeFiles/circuit.dir/src/qcircuit.cxx.o: CMakeFiles/circuit.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ben/github/l2-MBQC-Qubit-Reuse/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/circuit.dir/src/qcircuit.cxx.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/circuit.dir/src/qcircuit.cxx.o -MF CMakeFiles/circuit.dir/src/qcircuit.cxx.o.d -o CMakeFiles/circuit.dir/src/qcircuit.cxx.o -c /Users/ben/github/l2-MBQC-Qubit-Reuse/src/qcircuit.cxx

CMakeFiles/circuit.dir/src/qcircuit.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/circuit.dir/src/qcircuit.cxx.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ben/github/l2-MBQC-Qubit-Reuse/src/qcircuit.cxx > CMakeFiles/circuit.dir/src/qcircuit.cxx.i

CMakeFiles/circuit.dir/src/qcircuit.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/circuit.dir/src/qcircuit.cxx.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ben/github/l2-MBQC-Qubit-Reuse/src/qcircuit.cxx -o CMakeFiles/circuit.dir/src/qcircuit.cxx.s

# Object files for target circuit
circuit_OBJECTS = \
"CMakeFiles/circuit.dir/src/qcircuit.cxx.o"

# External object files for target circuit
circuit_EXTERNAL_OBJECTS =

libcircuit.a: CMakeFiles/circuit.dir/src/qcircuit.cxx.o
libcircuit.a: CMakeFiles/circuit.dir/build.make
libcircuit.a: CMakeFiles/circuit.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/ben/github/l2-MBQC-Qubit-Reuse/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libcircuit.a"
	$(CMAKE_COMMAND) -P CMakeFiles/circuit.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/circuit.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/circuit.dir/build: libcircuit.a
.PHONY : CMakeFiles/circuit.dir/build

CMakeFiles/circuit.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/circuit.dir/cmake_clean.cmake
.PHONY : CMakeFiles/circuit.dir/clean

CMakeFiles/circuit.dir/depend:
	cd /Users/ben/github/l2-MBQC-Qubit-Reuse/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ben/github/l2-MBQC-Qubit-Reuse /Users/ben/github/l2-MBQC-Qubit-Reuse /Users/ben/github/l2-MBQC-Qubit-Reuse/build /Users/ben/github/l2-MBQC-Qubit-Reuse/build /Users/ben/github/l2-MBQC-Qubit-Reuse/build/CMakeFiles/circuit.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/circuit.dir/depend

