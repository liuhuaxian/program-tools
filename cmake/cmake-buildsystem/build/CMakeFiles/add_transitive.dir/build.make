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
include CMakeFiles/add_transitive.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/add_transitive.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/add_transitive.dir/flags.make

CMakeFiles/add_transitive.dir/add.o: CMakeFiles/add_transitive.dir/flags.make
CMakeFiles/add_transitive.dir/add.o: ../add.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/add_transitive.dir/add.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/add_transitive.dir/add.o -c /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/add.c

CMakeFiles/add_transitive.dir/add.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/add_transitive.dir/add.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/add.c > CMakeFiles/add_transitive.dir/add.i

CMakeFiles/add_transitive.dir/add.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/add_transitive.dir/add.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/add.c -o CMakeFiles/add_transitive.dir/add.s

# Object files for target add_transitive
add_transitive_OBJECTS = \
"CMakeFiles/add_transitive.dir/add.o"

# External object files for target add_transitive
add_transitive_EXTERNAL_OBJECTS =

libadd_transitive.a: CMakeFiles/add_transitive.dir/add.o
libadd_transitive.a: CMakeFiles/add_transitive.dir/build.make
libadd_transitive.a: CMakeFiles/add_transitive.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libadd_transitive.a"
	$(CMAKE_COMMAND) -P CMakeFiles/add_transitive.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/add_transitive.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/add_transitive.dir/build: libadd_transitive.a

.PHONY : CMakeFiles/add_transitive.dir/build

CMakeFiles/add_transitive.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/add_transitive.dir/cmake_clean.cmake
.PHONY : CMakeFiles/add_transitive.dir/clean

CMakeFiles/add_transitive.dir/depend:
	cd /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lhx/work/github/program-tools/cmake/cmake-buildsystem /home/lhx/work/github/program-tools/cmake/cmake-buildsystem /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build /home/lhx/work/github/program-tools/cmake/cmake-buildsystem/build/CMakeFiles/add_transitive.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/add_transitive.dir/depend

