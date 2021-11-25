#include "../slam/slam.h"

#include "mona/targets/window.hpp"
#include "../io.hpp"

#include <iostream>

using namespace gl;
using namespace mona::targets;


auto init_gl()
{
    gl::init();

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


auto init_window(const window::params p)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    auto* window = glfwCreateWindow(p.w, p.h, p.title.c_str(), nullptr, nullptr);

    if(window == nullptr)
    {
        throw std::runtime_error("failed to create window");
    }
    glfwMakeContextCurrent(window);

    return window;
}

auto add_debug_callback()
{
    gl::glDebugMessageCallback([]
    (gl::GLenum source, gl::GLenum type, gl::GLuint id, gl::GLenum severity, gl::GLsizei length,
                const gl::GLchar *message, const void *userParam)
    {
        if(severity > GL_DEBUG_SEVERITY_NOTIFICATION)
        {
            std::cerr << message << '\n';
            //assert(false);
        }
    }, nullptr);
}


window::window(params p)
{
    handle = init_window(p);
    init_gl();
    add_debug_callback();
}


window::~window()
{
    glfwDestroyWindow(handle);
    glfwTerminate();
}


auto window::begin_frame() -> void
{
    int w, h;
    glfwGetWindowSize(handle, &w, &h);
    glViewport(0, 0, w, h);
}


auto window::end_frame() -> void
{

}


auto window::active() -> bool
{
    return glfwWindowShouldClose(handle) == false;
}


auto window::draw() -> void
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto a = area();
    glViewport(a.x, a.y, a.w, a.h);

    if(src)
    {
        src->draw(a);
        src = mona::exchange(nullptr, src);
    }


    glfwPollEvents();
    glfwSwapBuffers(handle);
}


auto window::area() -> mona::rect
{
    int w, h;
    glfwGetWindowSize(handle, &w, &h);
    return {0.f, 0.f, static_cast<float>(w), static_cast<float>(h)};
}

auto window::submit(const mona::source& src) -> void
{
    this->src = mona::exchange(&src, this->src);
}

auto window::get_camera_control() -> camera_input_control
{
    return camera_input_control(handle);
}