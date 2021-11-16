cmake_minimum_required(VERSION 3.0)

project(surface_plot)

add_executable(surface_plot main.cpp)

target_compile_features(surface_plot PRIVATE cxx_std_17)
target_link_libraries(surface_plot mona)