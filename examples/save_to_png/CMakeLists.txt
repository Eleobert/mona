cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(save_to_png)

set(CMAKE_CXX_FLAGS "-O0 -g")
add_executable(save_to_png main.cpp)

target_compile_features(save_to_png PRIVATE cxx_std_20)
target_include_directories(save_to_png PUBLIC ../include ..)
target_link_libraries(save_to_png mona glm::glm)