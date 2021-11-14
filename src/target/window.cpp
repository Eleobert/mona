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
            std::cerr << message << '\n';
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

auto window::control_camera(mona::camera cam) -> mona::camera
{
    constexpr float scroll_sens = 0.08;
    constexpr float mov_sens = 0.08;
    constexpr float min_dist = 0.1f;

    const auto pos = get_mouse_pos(handle);

    auto [cam_dist, cam_yaw, cam_pitch] = cam.pos();

     if(glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
     {
         const auto delta = mouse_pos - pos;

         auto yaw   = -delta.x * mov_sens;
         auto pitch = -delta.y * mov_sens;
         pitch = std::abs(cam_pitch + pitch) < 89.f? pitch: 0;
         cam.rotate(yaw, pitch);
     }
     mouse_pos = pos;

    auto dist_inc = get_mouse_scroll(handle) * scroll_sens;
    dist_inc = dist_inc + cam_dist < min_dist? 0: dist_inc;
    cam.zoom(dist_inc);

    auto [w, h] = this->size();
    cam.view_port = {0, 0, w, h};

    return cam;
}


auto window::begin_frame() -> void
{
    // auto bg = background_color / 255.f; 
    //glClearColor(bg.r, bg.g, bg.b, 1);

    clear = true;
}


auto window::end_frame() -> void
{

}


auto window::active() -> bool
{
    return glfwWindowShouldClose(handle) == false;
}


auto window::size() -> std::tuple<float, float>
{
    int w, h;
    glfwGetWindowSize(handle, &w, &h);
    return {w, h};
}

auto window::draw() -> void
{
    glfwPollEvents();
    glfwSwapBuffers(handle);

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
