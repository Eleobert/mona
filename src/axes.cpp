#include "mona/axes.hpp"
#include "slam/slam.h"
#include "mona/rect.hpp"
#include "mona/utility.hpp"
#include "mona/dots.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <limits>
#include <algorithm>
#include <span>

using namespace gl;


// return the plot area viewport
auto get_viewport(const mona::axes::params& par, const mona::rect& target_vp) -> mona::rect
{
    auto x = target_vp.x + par.padding_left * target_vp.w / 2.f;
    auto y = target_vp.y + par.padding_down * target_vp.h / 2.f;
    auto w = (2 - (par.padding_left + par.padding_right)) * target_vp.w / 2.f;
    auto h = (2 - (par.padding_up + par.padding_down)) * target_vp.h / 2.f;
    return {x, y, w, h};
}


mona::axes::axes(params par): par(par), trenderer("../../../res/fonts/Quivira.otf", 20)
{

}


auto draw_y_ticks(text_renderer& r, const mona::rect port, const arma::fvec& yy, float pad = 15)
{
    auto pos = glm::vec2(port.x, port.y);
    auto step = port.h / (yy.size() - 1);

    for(auto y: yy)
    {
        char buffer[50];
        sprintf(buffer, "%.3f", y);
        r.render(buffer, {pos.x - pad, pos.y}, 0.7, {0, 0, 0}, {1, 0});
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
        r.render(buffer, {pos.x, pos.y - pad}, 0.7, {0, 0, 0}, {0, 1});
        pos.x += step;
    }
}


auto get_plot_span(const std::vector<mona::line>& ls, const std::vector<mona::dots>& ds)
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

    for(auto& d: ds)
    {
        auto span = d.span();
        res.x = std::min(res.x, span.x);
        res.w = std::max(res.w, span.w);
        res.y = std::min(res.y, span.y);
        res.h = std::max(res.h, span.h);
    }
    return res;
}

auto add_tick_guides(std::span<float> sp)
{

}

auto update_port(mona::line& l, mona::rect vp, const mona::axes::params& par)
{
    auto x1 = vp.x;
    auto x2 = vp.x + vp.w;
    auto y1 = vp.y;
    auto y2 = vp.y + vp.h;

    auto port_x = arma::fvec(8 + par.n_bins * 4);
    auto port_y = arma::fvec(8 + par.n_bins * 4);
    port_x.subvec(0, 7) = arma::fvec({x1, x2, x2, x2, x2, x1, x1, x1});
    port_y.subvec(0, 7) = arma::fvec({y1, y1, y1, y2, y2, y2, y2, y1});

    auto x_step = (x2 - x1) / (par.n_bins - 1.f);
    auto y_step = (y2 - y1) / (par.n_bins - 1.f);

    for(auto i = 0; i < par.n_bins; i++)
    {
        port_x[2 * i + 8] = x1;
        port_x[2 * i + 9] = x1 - 10;
        port_y[2 * i + 8] = y1 + y_step * i;
        port_y[2 * i + 9] = y1 + y_step * i;
    }

    auto o = 2 * par.n_bins + 8;
    for(auto i = 0; i < par.n_bins; i++)
    {
        port_y[2 * i + o + 0] = y1;
        port_y[2 * i + o + 1] = y1 - 10;
        port_x[2 * i + o + 0] = x1 + x_step * i;
        port_x[2 * i + o + 1] = x1 + x_step * i;
    }

    if(l.empty())
    {
        // line is being created by the first time
        l = mona::line(port_x, port_y);
    }
    else
    {
        l.reset(port_x, port_y);
    }
    l.set_strip(false);
}


auto mona::axes::draw(const camera& cam, mona::targets::target& t) -> void
{
    t.begin_frame();

    glm::ivec4 prev_viewport;
    glGetIntegerv(GL_VIEWPORT, glm::value_ptr(prev_viewport));

    auto t_vp = t.viewport(); // target viewport
    auto vp = get_viewport(par, t.viewport()); // plot viewport

    if(t_vp != prev_target_viewport || port.empty())
    {
        update_port(port, vp, par);
    }

    auto t_ortho = glm::ortho(t_vp.x, t_vp.x + t_vp.w, t_vp.y, t_vp.y + t_vp.h);
    port.draw(t_ortho);
    trenderer.s.set_uniform("projection", t_ortho);

    auto span = get_plot_span(ls, ds);
    draw_x_ticks(trenderer, vp, mona::linspace(span.x, span.w, par.n_bins));
    draw_y_ticks(trenderer, vp, mona::linspace(span.y, span.h, par.n_bins));

    glViewport(vp.x, vp.y, vp.w, vp.h);

    auto ortho = glm::ortho(span.x, span.w, span.y, span.h);

    for(auto& l: ls)
    {
       l.draw(ortho);
    }

    for(auto& d: ds)
    {
        d.draw(ortho);
    }

    glViewport(prev_viewport.x, prev_viewport.y, prev_viewport.z, prev_viewport.w);

    t.end_frame();
    ls.clear();
}



auto mona::axes::submit(const mona::line& l) -> void
{
    this->ls.emplace_back(l);
}

auto mona::axes::submit(const mona::dots& l) -> void
{
    this->ds.emplace_back(l);
}
