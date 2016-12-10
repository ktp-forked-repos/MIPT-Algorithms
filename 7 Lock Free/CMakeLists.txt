cmake_minimum_required(VERSION 3.6)
project(7_Lock_Free)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-DLOCAL -g")
set(SOURCE_FILES
        main.cpp)

add_executable(7_Lock_Free ${SOURCE_FILES})