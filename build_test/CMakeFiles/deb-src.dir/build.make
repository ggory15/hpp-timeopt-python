# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ggory15/git/timeopt

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ggory15/git/timeopt/build_test

# Utility rule file for deb-src.

# Include the progress variables for this target.
include CMakeFiles/deb-src.dir/progress.make

CMakeFiles/deb-src:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ggory15/git/timeopt/build_test/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating source Debian package..."
	cd /home/ggory15/git/timeopt && git-buildpackage --git-debian-branch=debian

deb-src: CMakeFiles/deb-src
deb-src: CMakeFiles/deb-src.dir/build.make

.PHONY : deb-src

# Rule to build all files generated by this target.
CMakeFiles/deb-src.dir/build: deb-src

.PHONY : CMakeFiles/deb-src.dir/build

CMakeFiles/deb-src.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/deb-src.dir/cmake_clean.cmake
.PHONY : CMakeFiles/deb-src.dir/clean

CMakeFiles/deb-src.dir/depend:
	cd /home/ggory15/git/timeopt/build_test && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ggory15/git/timeopt /home/ggory15/git/timeopt /home/ggory15/git/timeopt/build_test /home/ggory15/git/timeopt/build_test /home/ggory15/git/timeopt/build_test/CMakeFiles/deb-src.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/deb-src.dir/depend

