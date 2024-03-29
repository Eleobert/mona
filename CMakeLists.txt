cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(mona)

set(CMAKE_CXX_FLAGS "-O0 -g")

find_package(armadillo REQUIRED)

find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)

add_subdirectory(examples)

add_library(mona SHARED
        include/mona/internal/grid.hpp
        include/mona/internal/text_renderer.hpp
        include/mona/line.hpp
        src/target/window.cpp
        src/slam/slam.cpp
        src/slam/slam.h
        src/surface_mesh.cpp
        src/camera.cpp
        src/shader.cpp
        src/io.cpp
        src/axes3.cpp
        src/text_renderer.cpp
        src/line_renderer.cpp include/mona/axes.hpp src/axes.cpp src/line.cpp
        include/mona/dots.hpp
        src/dots.cpp src/target/viewport.cpp include/mona/source.hpp
        src/camera_input_control.cpp include/mona/camera_input_control.hpp
        include/mona/camera_control.hpp include/mona/camera_steady_control.hpp
        include/mona/targets/png.hpp src/target/png.cpp src/stb/stb.cpp
        src/stb/stb.hpp)

target_compile_definitions(mona PUBLIC ARMA_DONT_USE_HDF5)
target_compile_features(mona PRIVATE cxx_std_20)
target_include_directories(mona PUBLIC include)
target_link_libraries(mona PRIVATE dl Freetype::Freetype PUBLIC glm::glm glfw armadillo::armadillo)