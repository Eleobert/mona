cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(grid_view)

set(CMAKE_CXX_FLAGS "-O0 -g")
add_executable(grid_view main.cpp)

target_compile_features(grid_view PRIVATE cxx_std_20)
target_include_directories(grid_view PUBLIC ../include ..)
target_link_libraries(grid_view mona glm::glm)