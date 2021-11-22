# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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
CMAKE_COMMAND = /home/lxm/Apps/clion-2021.2.1/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/lxm/Apps/clion-2021.2.1/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lxm/Documents/Code/redis-study

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lxm/Documents/Code/redis-study

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/home/lxm/Apps/clion-2021.2.1/bin/cmake/linux/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/home/lxm/Apps/clion-2021.2.1/bin/cmake/linux/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/lxm/Documents/Code/redis-study/CMakeFiles /home/lxm/Documents/Code/redis-study//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/lxm/Documents/Code/redis-study/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named redis_study

# Build rule for target.
redis_study: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 redis_study
.PHONY : redis_study

# fast build rule for target.
redis_study/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/redis_study.dir/build.make CMakeFiles/redis_study.dir/build
.PHONY : redis_study/fast

main.o: main.c.o
.PHONY : main.o

# target to build an object file
main.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/redis_study.dir/build.make CMakeFiles/redis_study.dir/main.c.o
.PHONY : main.c.o

main.i: main.c.i
.PHONY : main.i

# target to preprocess a source file
main.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/redis_study.dir/build.make CMakeFiles/redis_study.dir/main.c.i
.PHONY : main.c.i

main.s: main.c.s
.PHONY : main.s

# target to generate assembly for a file
main.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/redis_study.dir/build.make CMakeFiles/redis_study.dir/main.c.s
.PHONY : main.c.s

src/dict.o: src/dict.c.o
.PHONY : src/dict.o

# target to build an object file
src/dict.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/redis_study.dir/build.make CMakeFiles/redis_study.dir/src/dict.c.o
.PHONY : src/dict.c.o

src/dict.i: src/dict.c.i
.PHONY : src/dict.i

# target to preprocess a source file
src/dict.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/redis_study.dir/build.make CMakeFiles/redis_study.dir/src/dict.c.i
.PHONY : src/dict.c.i

src/dict.s: src/dict.c.s
.PHONY : src/dict.s

# target to generate assembly for a file
src/dict.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/redis_study.dir/build.make CMakeFiles/redis_study.dir/src/dict.c.s
.PHONY : src/dict.c.s

src/redis_zskip.o: src/redis_zskip.c.o
.PHONY : src/redis_zskip.o

# target to build an object file
src/redis_zskip.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/redis_study.dir/build.make CMakeFiles/redis_study.dir/src/redis_zskip.c.o
.PHONY : src/redis_zskip.c.o

src/redis_zskip.i: src/redis_zskip.c.i
.PHONY : src/redis_zskip.i

# target to preprocess a source file
src/redis_zskip.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/redis_study.dir/build.make CMakeFiles/redis_study.dir/src/redis_zskip.c.i
.PHONY : src/redis_zskip.c.i

src/redis_zskip.s: src/redis_zskip.c.s
.PHONY : src/redis_zskip.s

# target to generate assembly for a file
src/redis_zskip.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/redis_study.dir/build.make CMakeFiles/redis_study.dir/src/redis_zskip.c.s
.PHONY : src/redis_zskip.c.s

src/sds.o: src/sds.c.o
.PHONY : src/sds.o

# target to build an object file
src/sds.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/redis_study.dir/build.make CMakeFiles/redis_study.dir/src/sds.c.o
.PHONY : src/sds.c.o

src/sds.i: src/sds.c.i
.PHONY : src/sds.i

# target to preprocess a source file
src/sds.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/redis_study.dir/build.make CMakeFiles/redis_study.dir/src/sds.c.i
.PHONY : src/sds.c.i

src/sds.s: src/sds.c.s
.PHONY : src/sds.s

# target to generate assembly for a file
src/sds.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/redis_study.dir/build.make CMakeFiles/redis_study.dir/src/sds.c.s
.PHONY : src/sds.c.s

src/sort.o: src/sort.c.o
.PHONY : src/sort.o

# target to build an object file
src/sort.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/redis_study.dir/build.make CMakeFiles/redis_study.dir/src/sort.c.o
.PHONY : src/sort.c.o

src/sort.i: src/sort.c.i
.PHONY : src/sort.i

# target to preprocess a source file
src/sort.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/redis_study.dir/build.make CMakeFiles/redis_study.dir/src/sort.c.i
.PHONY : src/sort.c.i

src/sort.s: src/sort.c.s
.PHONY : src/sort.s

# target to generate assembly for a file
src/sort.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/redis_study.dir/build.make CMakeFiles/redis_study.dir/src/sort.c.s
.PHONY : src/sort.c.s

src/zskiplist.o: src/zskiplist.c.o
.PHONY : src/zskiplist.o

# target to build an object file
src/zskiplist.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/redis_study.dir/build.make CMakeFiles/redis_study.dir/src/zskiplist.c.o
.PHONY : src/zskiplist.c.o

src/zskiplist.i: src/zskiplist.c.i
.PHONY : src/zskiplist.i

# target to preprocess a source file
src/zskiplist.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/redis_study.dir/build.make CMakeFiles/redis_study.dir/src/zskiplist.c.i
.PHONY : src/zskiplist.c.i

src/zskiplist.s: src/zskiplist.c.s
.PHONY : src/zskiplist.s

# target to generate assembly for a file
src/zskiplist.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/redis_study.dir/build.make CMakeFiles/redis_study.dir/src/zskiplist.c.s
.PHONY : src/zskiplist.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... redis_study"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... src/dict.o"
	@echo "... src/dict.i"
	@echo "... src/dict.s"
	@echo "... src/redis_zskip.o"
	@echo "... src/redis_zskip.i"
	@echo "... src/redis_zskip.s"
	@echo "... src/sds.o"
	@echo "... src/sds.i"
	@echo "... src/sds.s"
	@echo "... src/sort.o"
	@echo "... src/sort.i"
	@echo "... src/sort.s"
	@echo "... src/zskiplist.o"
	@echo "... src/zskiplist.i"
	@echo "... src/zskiplist.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system
