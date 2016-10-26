cmake_minimum_required(VERSION 3.6)
project(3_Synchronization)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLOCAL -std=c++14 -pthread -g")

set(SOURCE_FILES robot_semaphore.cpp)
add_executable(3_Synchronization ${SOURCE_FILES})