#include <mona/camera_control.hpp>
#include "io.hpp"
#include <iostream>

auto mona::camera_control::operator()(mona::rect area) -> camera_control::control
{

//    if(not handle) // not good
//    {
//        return c;
//    }
    const auto pos = get_mouse_pos(handle);
    if(not mona::is_in(pos, area))
    {
        return control();
    }

    auto c = control();
    if(glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        const auto delta = pos - mouse_pos;

        c.yaw   = delta.x * mov_sens;
        c.pitch = delta.y * mov_sens;
    }
    mouse_pos = pos;

    c.zoom = get_mouse_scroll(handle) * zoom_sens;

    return c;
}
