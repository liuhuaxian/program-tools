# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lhx/work/github/program-tools/cmake/cmake-buildsystem

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build

# Include any dependencies generated for this target.
include CMakeFiles/uncompatible_string_main.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/uncompatible_string_main.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/uncompatible_string_main.dir/flags.make

CMakeFiles/uncompatible_string_main.dir/main.o: CMakeFiles/uncompatible_string_main.dir/flags.make
CMakeFiles/uncompatible_string_main.dir/main.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/uncompatible_string_main.dir/main.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/uncompatible_string_main.dir/main.o -c /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/main.c

CMakeFiles/uncompatible_string_main.dir/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/uncompatible_string_main.dir/main.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/main.c > CMakeFiles/uncompatible_string_main.dir/main.i

CMakeFiles/uncompatible_string_main.dir/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/uncompatible_string_main.dir/main.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/main.c -o CMakeFiles/uncompatible_string_main.dir/main.s

# Object files for target uncompatible_string_main
uncompatible_string_main_OBJECTS = \
"CMakeFiles/uncompatible_string_main.dir/main.o"

# External object files for target uncompatible_string_main
uncompatible_string_main_EXTERNAL_OBJECTS =

uncompatible_string_main: CMakeFiles/uncompatible_string_main.dir/main.o
uncompatible_string_main: CMakeFiles/uncompatible_string_main.dir/build.make
uncompatible_string_main: libaddVersion2.so
uncompatible_string_main: libsubVersion3.so
uncompatible_string_main: CMakeFiles/uncompatible_string_main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable uncompatible_string_main"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/uncompatible_string_main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/uncompatible_string_main.dir/build: uncompatible_string_main

.PHONY : CMakeFiles/uncompatible_string_main.dir/build

CMakeFiles/uncompatible_string_main.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/uncompatible_string_main.dir/cmake_clean.cmake
.PHONY : CMakeFiles/uncompatible_string_main.dir/clean

CMakeFiles/uncompatible_string_main.dir/depend:
	cd /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lhx/work/github/program-tools/cmake/cmake-buildsystem /home/lhx/work/github/program-tools/cmake/cmake-buildsystem /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build/CMakeFiles/uncompatible_string_main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/uncompatible_string_main.dir/depend

