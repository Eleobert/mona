#pragma once

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

auto get_mouse_pos(GLFWwindow* window) -> glm::vec2;

auto get_mouse_scroll(GLFWwindow* window) -> float;
