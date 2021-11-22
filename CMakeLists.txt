cmake_minimum_required(VERSION 3.20.2)
project(redis_study C)

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
#SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

add_executable(
        redis_study
        main.c
        src/sds.c
        src/sds.h
        src/ziplist.c
        src/ziplist.h
        src/dict.c
        src/dict.h
        src/zskiplist.h
        src/zskiplist.c
        tests/ziplistTests.c
        tests/dictTests.c
)
