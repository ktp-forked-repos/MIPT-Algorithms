cmake_minimum_required(VERSION 3.6)
project(Flows)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DLOCAL -g")

set(SOURCE_FILES C.cpp)
add_executable(Flows ${SOURCE_FILES})