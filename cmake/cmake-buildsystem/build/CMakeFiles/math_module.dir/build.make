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
include CMakeFiles/math_module.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/math_module.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/math_module.dir/flags.make

CMakeFiles/math_module.dir/add.o: CMakeFiles/math_module.dir/flags.make
CMakeFiles/math_module.dir/add.o: ../add.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/math_module.dir/add.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/math_module.dir/add.o -c /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/add.c

CMakeFiles/math_module.dir/add.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/math_module.dir/add.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/add.c > CMakeFiles/math_module.dir/add.i

CMakeFiles/math_module.dir/add.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/math_module.dir/add.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/add.c -o CMakeFiles/math_module.dir/add.s

CMakeFiles/math_module.dir/sub.o: CMakeFiles/math_module.dir/flags.make
CMakeFiles/math_module.dir/sub.o: ../sub.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/math_module.dir/sub.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/math_module.dir/sub.o -c /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/sub.c

CMakeFiles/math_module.dir/sub.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/math_module.dir/sub.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/sub.c > CMakeFiles/math_module.dir/sub.i

CMakeFiles/math_module.dir/sub.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/math_module.dir/sub.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/sub.c -o CMakeFiles/math_module.dir/sub.s

# Object files for target math_module
math_module_OBJECTS = \
"CMakeFiles/math_module.dir/add.o" \
"CMakeFiles/math_module.dir/sub.o"

# External object files for target math_module
math_module_EXTERNAL_OBJECTS =

libmath_module.so: CMakeFiles/math_module.dir/add.o
libmath_module.so: CMakeFiles/math_module.dir/sub.o
libmath_module.so: CMakeFiles/math_module.dir/build.make
libmath_module.so: CMakeFiles/math_module.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C shared module libmath_module.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/math_module.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/math_module.dir/build: libmath_module.so

.PHONY : CMakeFiles/math_module.dir/build

CMakeFiles/math_module.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/math_module.dir/cmake_clean.cmake
.PHONY : CMakeFiles/math_module.dir/clean

CMakeFiles/math_module.dir/depend:
	cd /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lhx/work/github/program-tools/cmake/cmake-buildsystem /home/lhx/work/github/program-tools/cmake/cmake-buildsystem /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build/CMakeFiles/math_module.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/math_module.dir/depend

