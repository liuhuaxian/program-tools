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
include CMakeFiles/MyFramework.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MyFramework.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MyFramework.dir/flags.make

CMakeFiles/MyFramework.dir/add.o: CMakeFiles/MyFramework.dir/flags.make
CMakeFiles/MyFramework.dir/add.o: ../add.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/MyFramework.dir/add.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/MyFramework.dir/add.o -c /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/add.c

CMakeFiles/MyFramework.dir/add.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/MyFramework.dir/add.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/add.c > CMakeFiles/MyFramework.dir/add.i

CMakeFiles/MyFramework.dir/add.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/MyFramework.dir/add.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/add.c -o CMakeFiles/MyFramework.dir/add.s

CMakeFiles/MyFramework.dir/sub.o: CMakeFiles/MyFramework.dir/flags.make
CMakeFiles/MyFramework.dir/sub.o: ../sub.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/MyFramework.dir/sub.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/MyFramework.dir/sub.o -c /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/sub.c

CMakeFiles/MyFramework.dir/sub.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/MyFramework.dir/sub.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/sub.c > CMakeFiles/MyFramework.dir/sub.i

CMakeFiles/MyFramework.dir/sub.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/MyFramework.dir/sub.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/sub.c -o CMakeFiles/MyFramework.dir/sub.s

# Object files for target MyFramework
MyFramework_OBJECTS = \
"CMakeFiles/MyFramework.dir/add.o" \
"CMakeFiles/MyFramework.dir/sub.o"

# External object files for target MyFramework
MyFramework_EXTERNAL_OBJECTS =

libMyFramework.so: CMakeFiles/MyFramework.dir/add.o
libMyFramework.so: CMakeFiles/MyFramework.dir/sub.o
libMyFramework.so: CMakeFiles/MyFramework.dir/build.make
libMyFramework.so: CMakeFiles/MyFramework.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C shared library libMyFramework.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MyFramework.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MyFramework.dir/build: libMyFramework.so

.PHONY : CMakeFiles/MyFramework.dir/build

CMakeFiles/MyFramework.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MyFramework.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MyFramework.dir/clean

CMakeFiles/MyFramework.dir/depend:
	cd /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lhx/work/github/program-tools/cmake/cmake-buildsystem /home/lhx/work/github/program-tools/cmake/cmake-buildsystem /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build/CMakeFiles/MyFramework.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MyFramework.dir/depend

