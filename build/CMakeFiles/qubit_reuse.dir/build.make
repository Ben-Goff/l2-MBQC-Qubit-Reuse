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
CMAKE_SOURCE_DIR = /Users/ben/GitHub/l2-MBQC-Qubit-Reuse

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ben/GitHub/l2-MBQC-Qubit-Reuse/build

# Include any dependencies generated for this target.
include CMakeFiles/qubit_reuse.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/qubit_reuse.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/qubit_reuse.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/qubit_reuse.dir/flags.make

CMakeFiles/qubit_reuse.dir/src/main.cxx.o: CMakeFiles/qubit_reuse.dir/flags.make
CMakeFiles/qubit_reuse.dir/src/main.cxx.o: /Users/ben/GitHub/l2-MBQC-Qubit-Reuse/src/main.cxx
CMakeFiles/qubit_reuse.dir/src/main.cxx.o: CMakeFiles/qubit_reuse.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ben/GitHub/l2-MBQC-Qubit-Reuse/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/qubit_reuse.dir/src/main.cxx.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/qubit_reuse.dir/src/main.cxx.o -MF CMakeFiles/qubit_reuse.dir/src/main.cxx.o.d -o CMakeFiles/qubit_reuse.dir/src/main.cxx.o -c /Users/ben/GitHub/l2-MBQC-Qubit-Reuse/src/main.cxx

CMakeFiles/qubit_reuse.dir/src/main.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/qubit_reuse.dir/src/main.cxx.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ben/GitHub/l2-MBQC-Qubit-Reuse/src/main.cxx > CMakeFiles/qubit_reuse.dir/src/main.cxx.i

CMakeFiles/qubit_reuse.dir/src/main.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/qubit_reuse.dir/src/main.cxx.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ben/GitHub/l2-MBQC-Qubit-Reuse/src/main.cxx -o CMakeFiles/qubit_reuse.dir/src/main.cxx.s

# Object files for target qubit_reuse
qubit_reuse_OBJECTS = \
"CMakeFiles/qubit_reuse.dir/src/main.cxx.o"

# External object files for target qubit_reuse
qubit_reuse_EXTERNAL_OBJECTS =

qubit_reuse: CMakeFiles/qubit_reuse.dir/src/main.cxx.o
qubit_reuse: CMakeFiles/qubit_reuse.dir/build.make
qubit_reuse: /Users/ben/GitHub/l2-MBQC-Qubit-Reuse/ogdf-master/libOGDF.a
qubit_reuse: libcircuit.a
qubit_reuse: libgate.a
qubit_reuse: /Users/ben/GitHub/l2-MBQC-Qubit-Reuse/ogdf-master/libCOIN.a
qubit_reuse: CMakeFiles/qubit_reuse.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/ben/GitHub/l2-MBQC-Qubit-Reuse/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable qubit_reuse"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/qubit_reuse.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/qubit_reuse.dir/build: qubit_reuse
.PHONY : CMakeFiles/qubit_reuse.dir/build

CMakeFiles/qubit_reuse.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/qubit_reuse.dir/cmake_clean.cmake
.PHONY : CMakeFiles/qubit_reuse.dir/clean

CMakeFiles/qubit_reuse.dir/depend:
	cd /Users/ben/GitHub/l2-MBQC-Qubit-Reuse/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ben/GitHub/l2-MBQC-Qubit-Reuse /Users/ben/GitHub/l2-MBQC-Qubit-Reuse /Users/ben/GitHub/l2-MBQC-Qubit-Reuse/build /Users/ben/GitHub/l2-MBQC-Qubit-Reuse/build /Users/ben/GitHub/l2-MBQC-Qubit-Reuse/build/CMakeFiles/qubit_reuse.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/qubit_reuse.dir/depend

