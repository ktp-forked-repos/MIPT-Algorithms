cmake_minimum_required(VERSION 3.6)
project(5_Thread_Pool)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -pthread -DLOCAL -Wall -Werror -Wextra")

set(SOURCE_FILES parallel_sort.cpp)
add_executable(5_Thread_Pool ${SOURCE_FILES})
add_executable(Test main2.cpp)