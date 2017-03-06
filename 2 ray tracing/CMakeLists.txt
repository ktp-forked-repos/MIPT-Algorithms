cmake_minimum_required(VERSION 3.6)
project(ray_tracing)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-DLOCAL -g -lglut -lGL -lGLU")

include_directories(/home/dima/seminars/includes)
set(SOURCE_FILES main.cpp geometry.h glut.h matrix.h ray_tracing.h objects.h material.h light.h random.h pretty_print.h)
add_executable(ray_tracing ${SOURCE_FILES})