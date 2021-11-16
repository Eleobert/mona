#pragma once

#include "target.hpp"
#include "mona/colors.hpp"

#include <string>
#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>


namespace mona::targets
{
    class window: public target
    {
        glm::vec2 mouse_pos;
        GLFWwindow* handle = nullptr;

    public:

        struct params
        {
            params() {}
            int w = 570, h = 510;
            std::string title = "mona";
        };

        window(params p = params());
        ~window();

        auto control_camera(mona::camera cam) -> mona::camera override;
        auto active() -> bool override;
        auto begin_frame() -> void override;
        auto end_frame() -> void override;
        auto draw() -> void override;

        auto size() -> std::tuple<float, float>;
    };
}