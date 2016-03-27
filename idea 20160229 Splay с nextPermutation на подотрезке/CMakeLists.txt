cmake_minimum_required(VERSION 3.3)
project(idea_20160229_Splay)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

set(SOURCE_FILES main.cpp SplayStruct.cpp SplayNode.cpp SplayOperations.cpp VectorStruct.cpp AbstractGenerator.cpp)
add_executable(idea_20160229_Splay ${SOURCE_FILES})