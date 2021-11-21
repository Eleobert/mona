cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(scatter_plot)

set(CMAKE_CXX_FLAGS "-O0 -g")
add_executable(scatter_plot main.cpp)

target_compile_features(scatter_plot PRIVATE cxx_std_20)
target_include_directories(scatter_plot PUBLIC ../include ..)
target_link_libraries(scatter_plot mona glm::glm)