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
include CMakeFiles/main_change_order.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/main_change_order.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main_change_order.dir/flags.make

CMakeFiles/main_change_order.dir/main.o: CMakeFiles/main_change_order.dir/flags.make
CMakeFiles/main_change_order.dir/main.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/main_change_order.dir/main.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/main_change_order.dir/main.o -c /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/main.c

CMakeFiles/main_change_order.dir/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main_change_order.dir/main.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/main.c > CMakeFiles/main_change_order.dir/main.i

CMakeFiles/main_change_order.dir/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main_change_order.dir/main.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/main.c -o CMakeFiles/main_change_order.dir/main.s

# Object files for target main_change_order
main_change_order_OBJECTS = \
"CMakeFiles/main_change_order.dir/main.o"

# External object files for target main_change_order
main_change_order_EXTERNAL_OBJECTS =

main_change_order: CMakeFiles/main_change_order.dir/main.o
main_change_order: CMakeFiles/main_change_order.dir/build.make
main_change_order: libdiv_transitive.a
main_change_order: libadd_transitive.a
main_change_order: libmath_transitive.a
main_change_order: libdiv_transitive.a
main_change_order: libadd_transitive.a
main_change_order: CMakeFiles/main_change_order.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable main_change_order"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main_change_order.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main_change_order.dir/build: main_change_order

.PHONY : CMakeFiles/main_change_order.dir/build

CMakeFiles/main_change_order.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/main_change_order.dir/cmake_clean.cmake
.PHONY : CMakeFiles/main_change_order.dir/clean

CMakeFiles/main_change_order.dir/depend:
	cd /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lhx/work/github/program-tools/cmake/cmake-buildsystem /home/lhx/work/github/program-tools/cmake/cmake-buildsystem /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build/CMakeFiles/main_change_order.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/main_change_order.dir/depend

