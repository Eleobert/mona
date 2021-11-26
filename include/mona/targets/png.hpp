#pragma once

#include <mona/source.hpp>
#include <GLFW/glfw3.h>

#include <string>

namespace mona
{
    class png
    {
        uint frame_buffer;
        GLFWwindow* dummy_window = nullptr;
        int w = 0, h = 0;
    public:
        png(int w, int h);
        ~png();
        auto save(source& src, const std::string& filename) -> void;
    };
};
