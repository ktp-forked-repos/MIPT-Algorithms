cmake_minimum_required(VERSION 3.6)
project(5_Thread_Pool)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -g -Wall -Wextra -DLOCAL")

set(SOURCE_FILES parallel_sort.cpp)
add_executable(Main ${SOURCE_FILES})