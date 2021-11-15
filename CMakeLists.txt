cmake_minimum_required(VERSION 3.20)
project(tutorial)

set(CMAKE_CXX_STANDARD 23)

add_executable(tutorial
        source/main.cpp
        source/params.h
        source/utils.cpp
        source/utils.h)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIR})
target_link_libraries(tutorial ${OpenCV_LIBS})