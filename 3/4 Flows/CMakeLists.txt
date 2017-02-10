cmake_minimum_required(VERSION 3.6)
project(4_Flows)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DLOCAL -g -Wall")

set(SOURCE_FILES L.cpp)
add_executable(4_Flows ${SOURCE_FILES})
#add_executable(Gen rand.cpp)