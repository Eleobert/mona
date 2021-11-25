#pragma once

#include "mona/rect.hpp"

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>


namespace mona
{
    class camera_control
    {
    public:
        struct control
        {
            float zoom  = 0.f;
            float yaw   = 0.f;
            float pitch = 0.f;
        };
        camera_control() = default;
        camera_control(GLFWwindow* handle)
        {
            this->handle = handle;
        }
        auto operator()(mona::rect area) -> control;

    private:
        GLFWwindow* handle = nullptr;
        float zoom_sens = 0.08;
        float mov_sens = 0.08;
        glm::vec2 mouse_pos;
    };
}