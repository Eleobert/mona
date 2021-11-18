#include "mona/axes.hpp"
#include "slam/slam.h"
#include "mona/rect.hpp"
#include "mona/utility.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <limits>
#include <algorithm>

using namespace gl;


auto add_ticks_guides(line_renderer& r, glm::vec2 a, glm::vec2 b, glm::vec3 w, int n)
{
    auto pos = glm::vec3(a, 0.f);
    auto step = glm::vec3((b - a) / (n - 1.f), 0.f);
    for(auto i = 0u; i < n; i++)
    {
        r.submit(pos, pos - w);
        pos += step;
    }
}

mona::axes::axes(params par): par(par), trenderer("../../../res/fonts/Quivira.otf", 15)
{
    auto x1 = par.padding_left - 1;
    auto x2 = 1 - par.padding_right;
    auto y1 = 1 - par.padding_up;
    auto y2 = par.padding_down - 1;

    port_boundary.submit({x1, y1, 0}, {x2, y1, 0});
    port_boundary.submit({x2, y1, 0}, {x2, y2, 0});
    port_boundary.submit({x2, y2, 0}, {x1, y2, 0});
    port_boundary.submit({x1, y2, 0}, {x1, y1, 0});

    constexpr auto w = 0.03;
    add_ticks_guides(port_boundary, {x1, y1}, {x1, y2}, {w, 0, 0}, par.n_bins);
    add_ticks_guides(port_boundary, {x1, y2}, {x2, y2}, {0, w, 0}, par.n_bins);

    port_boundary.gen_buffer();
}

auto draw_y_ticks(text_renderer& r, const mona::rect port, const arma::fvec& yy, float pad = 15)
{
    auto pos = glm::vec2(port.x, port.y);
    auto step = port.h / (yy.size() - 1);

    for(auto y: yy)
    {
        char buffer[50];
        sprintf(buffer, "%.3f", y);
        r.render(buffer, {pos.x - pad, pos.y}, 1, {0, 0, 0}, {1, 0});
        pos.y += step;
    }
}


auto draw_x_ticks(text_renderer& r, const mona::rect port, const arma::fvec& xx, float pad = 15)
{
    auto pos = glm::vec2(port.x, port.y);
    auto step = port.w / (xx.size() - 1);

    for(auto y: xx)
    {
        char buffer[50];
        sprintf(buffer, "%.3f", y);
        r.render(buffer, {pos.x, pos.y - pad}, 1, {0, 0, 0}, {0, 1});
        pos.x += step;
    }
}


// return the plot area viewport
auto get_viewport(const mona::axes::params& par, const mona::camera& cam) -> mona::rect
{
    auto x = cam.view_port.x + par.padding_left * cam.view_port.z / 2.f;
    auto y = cam.view_port.y + par.padding_down * cam.view_port.w / 2.f;
    auto w = (2 - (par.padding_left + par.padding_right)) * cam.view_port.z / 2.f;
    auto h = (2 - (par.padding_up + par.padding_down)) * cam.view_port.w / 2.f;
    return {x, y, w, h};
}


auto get_plot_span(const std::vector<mona::line>& ls)
{
    constexpr auto inf = std::numeric_limits<float>::infinity();
    auto res = mona::rect{inf, inf, -inf, -inf};

    for(auto& l: ls)
    {
        auto span = l.span();
        res.x = std::min(res.x, span.x);
        res.w = std::max(res.w, span.w);
        res.y = std::min(res.y, span.y);
        res.h = std::max(res.h, span.h);
    }
    return res;
}

auto mona::axes::draw(const camera& cam, mona::targets::target& t) -> void
{
    t.begin_frame();

    glm::ivec4 prev_viewport;
    glGetIntegerv(GL_VIEWPORT, glm::value_ptr(prev_viewport));

    port_boundary.draw(glm::mat4(1.f), mona::colors::black, 1.f);
    // TODO: viewport should come from target
    trenderer.s.set_uniform("projection", glm::ortho(0.f, cam.view_port.z, 0.f, cam.view_port.w));

    auto vp = get_viewport(par, cam);
    auto span = get_plot_span(ls);
    draw_x_ticks(trenderer, vp, mona::linspace(span.x, span.w, par.n_bins));
    draw_y_ticks(trenderer, vp, mona::linspace(span.y, span.h, par.n_bins));

    glViewport(vp.x, vp.y, vp.w, vp.h);

    auto ortho = glm::ortho(span.x, span.w, span.y, span.h);

    for(auto& l: ls)
    {
       l.draw(ortho);
    }

    glViewport(prev_viewport.x, prev_viewport.y, prev_viewport.z, prev_viewport.w);

    t.end_frame();
    ls.clear();
}



auto mona::axes::submit(const mona::line& l) -> void
{
    auto ref = this->ls.emplace_back(l);
}