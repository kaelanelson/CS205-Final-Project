# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/Simon/classes/CS205-Final-Project/Sequential

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Simon/classes/CS205-Final-Project/Sequential/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/prims.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/prims.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/prims.dir/flags.make

CMakeFiles/prims.dir/prims.cpp.o: CMakeFiles/prims.dir/flags.make
CMakeFiles/prims.dir/prims.cpp.o: ../prims.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Simon/classes/CS205-Final-Project/Sequential/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/prims.dir/prims.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/prims.dir/prims.cpp.o -c /Users/Simon/classes/CS205-Final-Project/Sequential/prims.cpp

CMakeFiles/prims.dir/prims.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/prims.dir/prims.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Simon/classes/CS205-Final-Project/Sequential/prims.cpp > CMakeFiles/prims.dir/prims.cpp.i

CMakeFiles/prims.dir/prims.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/prims.dir/prims.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Simon/classes/CS205-Final-Project/Sequential/prims.cpp -o CMakeFiles/prims.dir/prims.cpp.s

# Object files for target prims
prims_OBJECTS = \
"CMakeFiles/prims.dir/prims.cpp.o"

# External object files for target prims
prims_EXTERNAL_OBJECTS =

prims: CMakeFiles/prims.dir/prims.cpp.o
prims: CMakeFiles/prims.dir/build.make
prims: CMakeFiles/prims.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Simon/classes/CS205-Final-Project/Sequential/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable prims"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/prims.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/prims.dir/build: prims

.PHONY : CMakeFiles/prims.dir/build

CMakeFiles/prims.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/prims.dir/cmake_clean.cmake
.PHONY : CMakeFiles/prims.dir/clean

CMakeFiles/prims.dir/depend:
	cd /Users/Simon/classes/CS205-Final-Project/Sequential/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Simon/classes/CS205-Final-Project/Sequential /Users/Simon/classes/CS205-Final-Project/Sequential /Users/Simon/classes/CS205-Final-Project/Sequential/cmake-build-debug /Users/Simon/classes/CS205-Final-Project/Sequential/cmake-build-debug /Users/Simon/classes/CS205-Final-Project/Sequential/cmake-build-debug/CMakeFiles/prims.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/prims.dir/depend

