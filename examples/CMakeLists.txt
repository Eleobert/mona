cmake_minimum_required(VERSION 3.0)

project(surface_plot)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-O0 -g")

add_executable(surface_plot main.cpp)

target_compile_features(surface_plot PRIVATE cxx_std_17)
target_include_directories(surface_plot PUBLIC ../include ..)
target_link_libraries(surface_plot mona)