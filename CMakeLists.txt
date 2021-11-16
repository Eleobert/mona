cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(mona)

#set(CMAKE_CXX_FLAGS "-O0 -g")

#include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)
#find_package(armadillo REQUIRED)

add_subdirectory(examples)

add_library(mona SHARED
        include/mona/plot/surface.hpp
        include/mona/internal/grid.hpp
        include/mona/internal/text_renderer.hpp
        include/mona/line.hpp
        src/plot/surface.cpp
        src/target/window.cpp
        src/slam/slam.cpp
        src/slam/slam.h
        src/surface_mesh.cpp
        src/camera.cpp
        src/shader.cpp
        src/io.cpp
        src/axes3.cpp
        src/text_renderer.cpp
        src/line_renderer.cpp include/mona/axes.hpp src/axes.cpp src/line.cpp)

target_compile_features(mona PRIVATE cxx_std_20)
target_include_directories(mona PUBLIC include)
target_link_libraries(mona dl glfw Freetype::Freetype glm::glm)