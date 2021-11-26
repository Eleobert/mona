#include "../slam/slam.h"
#include <mona/targets/png.hpp>
#include <mona/source.hpp>

#include "../stb/stb.hpp"

#include <cstddef>
#include <vector>


auto init_dummy_window(int w, int h)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    GLFWwindow* offscreen_context = glfwCreateWindow(w, h, "", nullptr, nullptr);
    glfwMakeContextCurrent(offscreen_context);
    return offscreen_context;
}



auto create_frame_buffer(int w, int h)
{
    auto frame_buffer = 0u;
    glGenFramebuffers(1, &frame_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
    // create a color attachment texture
    auto texture_colorbuffer = 0u;
    glGenTextures(1, &texture_colorbuffer);
    glBindTexture(GL_TEXTURE_2D, texture_colorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_colorbuffer, 0);

    auto rbo = 0u;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return frame_buffer;
}


auto flip_vertically(std::vector<std::byte>& buffer, int w, int h, int n_channels)
{
    for(auto i = 0ul; i < h/2; i++)
    {
        for(auto j = 0ul; j < w * n_channels; j++)
        {
            std::swap(buffer[i * w * n_channels + j], buffer[(h - i - 1) * w * n_channels + j]);
        }
    }
}


mona::png::png(int w, int h): w(w), h(h)
{
    dummy_window = init_dummy_window(w, h);
    gl::init();
    auto frame_buffer = create_frame_buffer(w, h);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

mona::png::~png()
{
    glDeleteFramebuffers(1, &frame_buffer);
    glfwDestroyWindow(dummy_window);
}


auto mona::png::save(source& src, const std::string& filename) -> void
{
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    src.draw({0, 0, static_cast<float>(w), static_cast<float>(h)});

    constexpr auto n_channels = 3;
    auto buffer = std::vector<std::byte>(w * h * n_channels);
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
    flip_vertically(buffer, w, h, n_channels);
    stbi_write_png(filename.c_str(), w, h, n_channels, buffer.data(), w * n_channels);
}

