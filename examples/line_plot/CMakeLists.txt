cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(line_plot)

set(CMAKE_CXX_FLAGS "-O0 -g")
add_executable(line_plot main.cpp)

target_compile_features(line_plot PRIVATE cxx_std_17)
target_include_directories(line_plot PUBLIC ../include ..)
target_link_libraries(line_plot mona glm::glm)