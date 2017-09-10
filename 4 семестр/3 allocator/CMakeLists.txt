cmake_minimum_required(VERSION 3.7)
project(3_allocator)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-DLOCAL -g -pthread")

set(SOURCE_FILES main.cpp)
add_executable(3_allocator ${SOURCE_FILES})