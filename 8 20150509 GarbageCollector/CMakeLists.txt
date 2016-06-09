cmake_minimum_required(VERSION 3.5)
project(GarbageCollector)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp SmartObject.cpp GarbageCollector.cpp)
add_executable(GarbageCollector ${SOURCE_FILES})