#include "io.hpp"

auto get_mouse_scroll(GLFWwindow* window) -> float
{
    static auto initialized = false;
    static float prev_scroll = 0.f;
    static float scroll;

    if(initialized == false)
    {
        glfwSetScrollCallback(window, [](GLFWwindow* window, double x, double y)
        {
        scroll = y;
        });

        initialized = true;
    }
    auto res = scroll;
    scroll = 0;
    return res;
}

auto get_mouse_pos(GLFWwindow* window) -> glm::vec2
{
    static auto initialized = false;
    static glm::vec2 mouse_pos;

    if(initialized == false)
    {
        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y)
        {
        mouse_pos.x = x;
        mouse_pos.y = y;
        });

        initialized = true;
    }
    return mouse_pos;
}
