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
include CMakeFiles/gate.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/gate.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/gate.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gate.dir/flags.make

CMakeFiles/gate.dir/src-shared/gate.cxx.o: CMakeFiles/gate.dir/flags.make
CMakeFiles/gate.dir/src-shared/gate.cxx.o: /Users/ben/github/l2-MBQC-Qubit-Reuse/src-shared/gate.cxx
CMakeFiles/gate.dir/src-shared/gate.cxx.o: CMakeFiles/gate.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ben/github/l2-MBQC-Qubit-Reuse/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/gate.dir/src-shared/gate.cxx.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/gate.dir/src-shared/gate.cxx.o -MF CMakeFiles/gate.dir/src-shared/gate.cxx.o.d -o CMakeFiles/gate.dir/src-shared/gate.cxx.o -c /Users/ben/github/l2-MBQC-Qubit-Reuse/src-shared/gate.cxx

CMakeFiles/gate.dir/src-shared/gate.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gate.dir/src-shared/gate.cxx.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ben/github/l2-MBQC-Qubit-Reuse/src-shared/gate.cxx > CMakeFiles/gate.dir/src-shared/gate.cxx.i

CMakeFiles/gate.dir/src-shared/gate.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gate.dir/src-shared/gate.cxx.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ben/github/l2-MBQC-Qubit-Reuse/src-shared/gate.cxx -o CMakeFiles/gate.dir/src-shared/gate.cxx.s

# Object files for target gate
gate_OBJECTS = \
"CMakeFiles/gate.dir/src-shared/gate.cxx.o"

# External object files for target gate
gate_EXTERNAL_OBJECTS =

libgate.a: CMakeFiles/gate.dir/src-shared/gate.cxx.o
libgate.a: CMakeFiles/gate.dir/build.make
libgate.a: CMakeFiles/gate.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/ben/github/l2-MBQC-Qubit-Reuse/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libgate.a"
	$(CMAKE_COMMAND) -P CMakeFiles/gate.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gate.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/gate.dir/build: libgate.a
.PHONY : CMakeFiles/gate.dir/build

CMakeFiles/gate.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gate.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gate.dir/clean

CMakeFiles/gate.dir/depend:
	cd /Users/ben/github/l2-MBQC-Qubit-Reuse/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ben/github/l2-MBQC-Qubit-Reuse /Users/ben/github/l2-MBQC-Qubit-Reuse /Users/ben/github/l2-MBQC-Qubit-Reuse/build /Users/ben/github/l2-MBQC-Qubit-Reuse/build /Users/ben/github/l2-MBQC-Qubit-Reuse/build/CMakeFiles/gate.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/gate.dir/depend

