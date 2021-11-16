#include "mona/axes.hpp"
#include "slam/slam.h"

#include <glm/gtc/type_ptr.hpp>

#include <limits>
#include <algorithm>

using namespace gl;
constexpr auto inf = std::numeric_limits<float>::infinity();

mona::axes::axes(): x(inf, -inf), y(inf, -inf), bg_color(214, 255, 255)
{
    lrenderer.submit({-1,-1, 0}, { 1,-1, 0});
    lrenderer.submit({ 1,-1, 0}, { 1, 1, 0});
    lrenderer.submit({ 1, 1, 0}, {-1, 1, 0});
    lrenderer.submit({-1, 1, 0}, {-1,-1, 0});
    lrenderer.gen_buffer();
}


auto mona::axes::draw(const camera& cam, mona::targets::target& t) -> void
{
    t.begin_frame();

    glm::ivec4 prev_viewport;
    glGetIntegerv(GL_VIEWPORT, glm::value_ptr(prev_viewport));

    auto p = 0.15f;
    auto h_pad = p * cam.view_port.z;
    auto v_pad = p * cam.view_port.w;
    auto w = cam.view_port.z - 2 * h_pad;
    auto h = cam.view_port.w - 2 * v_pad;

    lrenderer.draw(glm::mat4(1.f), mona::colors::black);
    glViewport(h_pad, v_pad, w, h);
    auto c = bg_color /  255.f;
    glScissor(h_pad, v_pad, w, h);
    glClearColor(c.r, c.g, c.b, 1.f);
    glEnable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    auto ortho = glm::ortho(x.x, x.y, y.x, y.y);
    //s.set_uniform("mvp", ortho);

    for(auto& l: ls)
    {
        l.draw(ortho);
    }

    glScissor(prev_viewport.x, prev_viewport.y, prev_viewport.z, prev_viewport.w);
    glViewport(prev_viewport.x, prev_viewport.y, prev_viewport.z, prev_viewport.w);

    t.end_frame();
}


auto mona::axes::add(const mona::line& ls) -> void
{
    this->ls.push_back(ls);

    auto [x_min, x_max] = std::ranges::minmax(ls.x);
    auto [y_min, y_max] = std::ranges::minmax(ls.x);

    x.x = std::min(x.x, x_min);
    x.y = std::max(x.y, x_max);

    y.x = std::min(y.x, y_min);
    y.y = std::max(y.y, y_max);
}