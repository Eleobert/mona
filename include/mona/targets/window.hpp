#pragma once

#include "target.hpp"
#include "mona/colors.hpp"
#include <mona/source.hpp>
#include <mona/camera_input_control.hpp>

#include <string>
#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>


namespace mona::targets
{
    class window: public target
    {
        glm::vec2 mouse_pos;
        GLFWwindow* handle = nullptr;
        const mona::source* src = nullptr;

    public:

        struct params
        {
            params() {}
            int w = 570, h = 510;
            std::string title = "mona";
        };

        window(params p = params());
        ~window();
        auto get_camera_control() -> camera_input_control;
        auto active() -> bool override;
        auto begin_frame() -> void override;
        auto end_frame() -> void override;
        auto area() -> mona::rect  override;

        auto draw() -> void;
        auto submit(const mona::source& src) -> void;
    };
}