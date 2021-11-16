#include "mona/axes3.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_integer.hpp>


auto linspace(text_renderer& trenderer, const mona::camera& cam, glm::vec3 a, glm::vec3 b,
              float start, float end, int n, glm::vec2 font_origin)
{
    const auto coord_step = (b - a) / (static_cast<float>(n) - 1.f);
    const auto value_step = (end - start) / (n - 1.0);

    for(auto i = 0; i < n; i++)
    {
        auto p = glm::project(a, glm::mat4(1.f), cam.mvprsp(), cam.view_port);
        char buffer[50];
        sprintf(buffer, "%.3f", start);
        const auto size = trenderer.size(buffer, 1);
        trenderer.render(buffer, p, 1, {0, 0, 0}, font_origin);

        a += coord_step;
        start += value_step;
    }
}

#include <iostream>

mona::axes3::axes3(glm::vec2 x, glm::vec2 y, glm::vec2 z, int n): x(x), y(y), z(z), n(n),
    trenderer("../../res/fonts/Quivira.otf", 12)
{
    assert(n > 0);
    std::cout <<z.x << " " << z.y << "\n";
    bottom = grid({-1,-1,-1}, {-1,-1, 1}, { 1,-1,-1}, { 1,-1, 1}, n);
    up     = grid({ 1, 1,-1}, { 1, 1, 1}, {-1, 1,-1}, {-1, 1, 1}, n);
    left   = grid({-1, 1, 1}, {-1,-1, 1}, {-1, 1,-1}, {-1,-1,-1}, n);
    right  = grid({ 1, 1,-1}, { 1,-1,-1}, { 1, 1, 1}, { 1,-1, 1}, n);
    front  = grid({ 1,-1,-1}, { 1, 1,-1}, {-1,-1,-1}, {-1, 1,-1}, n);
    back   = grid({-1,-1, 1}, {-1, 1, 1}, { 1,-1, 1}, { 1, 1, 1}, n);
}

auto draw_grid(grid& g, glm::mat3 mvp) -> bool
{
    auto a = mvp * g.b - mvp * g.a;
    auto b = mvp * g.c - mvp * g.a;

    auto mat = glm::mat3(a, b, glm::vec3(1));
    // manual face culling: it is better to have mvp constructed from orthographic
    // projection matrix to avoid two parallel faces being displayed at the same time
    if(glm::determinant(mat) > 0)
    {
        g.draw();
        return true;
    }
    return false;
}


auto mona::axes3::draw(const camera& cam, mona::targets::target& t) -> void
{
    t.begin_frame();

    auto mvporth = cam.mvorth();
    auto mvpersp = cam.mvprsp();
    auto visible_bottom = draw_grid(bottom, mvporth);
    auto visible_up     = draw_grid(up, mvporth);
    auto visible_left   = draw_grid(left, mvporth);
    auto visible_right  = draw_grid(right, mvporth);
    auto visible_front  = draw_grid(front, mvporth);
    auto visible_back   = draw_grid(back, mvporth);

    auto num = [](bool b)
    {
        return b? 1.f: -1.f;
    };

    glm::vec3 x1, x2, y1, y2, z1, z2;
    glm::vec2 font_origin_x(0), font_origin_y(0), font_origin_z(0);

    x1.x = -1;
    x2.x = +1;
    x1.z = num(visible_front);
    x2.z = x1.z;
    x1.y = num(visible_up);
    x2.y = x1.y;

    y1.x = num(visible_left);
    y2.x = y1.x;
    y1.y = num(visible_up);
    y2.y = y1.y;
    y1.z = -1;
    y2.z = +1;

    // We know that there are two places where z line can be:
    //  a. opposite to x plane and adjacent to y plane
    //  b. opposite to y plane and adjacent to x plane
    // We select the one that is closer to the camera

    glm::vec3 z0(y1.x, 0, x1.z);

    auto p1 = glm::vec4(z0, 1); p1.x = -p1.x;
    auto p2 = glm::vec4(z0, 1); p2.z = -p2.z;

    z0 = (mvpersp * p1).z < (mvpersp * p2).z? p1: p2;
    z1 = z0;
    z2 = z0;
    z1.y = -1;
    z2.y = +1;

    font_origin_z.x = (mvpersp * glm::vec4(z0, 1)).x < 0? 1: -1;
    font_origin_z.y = visible_bottom? -1: 1; // avoids overlapping with x/y
    font_origin_x.x = (visible_right && visible_front || visible_left && visible_back)? -1: 1;
    font_origin_x.y = (visible_up)? -1: 1;
    font_origin_y.x = -font_origin_x.x;
    font_origin_y.y = font_origin_x.y;

    // auto sign = [](float x)
    // {
    //     return x < 0? -1: 1;
    // };

    // font_origin_z.x += sign(font_origin_z.x) * 0.6;
    // font_origin_y.x += sign(font_origin_y.x) * 1;
    // font_origin_x.x += sign(font_origin_x.x) * 1;


    auto ortho = glm::ortho(0.f, cam.view_port.z, 0.f, cam.view_port.w);
    trenderer.s.set_uniform("projection", ortho);


    linspace(trenderer, cam, x1, x2, x.x, x.y, n, font_origin_x);
    linspace(trenderer, cam, y1, y2, y.x, y.y, n, font_origin_y);
    linspace(trenderer, cam, z1, z2, z.x, z.y, n, font_origin_z);

    t.end_frame();
}

