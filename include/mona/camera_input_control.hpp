#pragma once

#include <mona/camera_control.hpp>

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>


namespace mona
{
    class camera_input_control: public camera_control
    {
    public:
        camera_input_control() = default;
        camera_input_control(GLFWwindow* handle)
        {
            this->handle = handle;
        }
        auto operator()(mona::rect area) -> control override;

    private:
        GLFWwindow* handle = nullptr;
        float zoom_sens = 0.08;
        float mov_sens = 0.08;
        glm::vec2 mouse_pos;
    };
}