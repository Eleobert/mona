#include "mona/axes3.hpp"

#include <glm/gtc/matrix_transform.hpp>


auto linspace(text_renderer& trenderer, glm::vec3 a, glm::vec3 b,
              float start, float end, int n, glm::vec2 font_origin, glm::mat4 mvp, mona::rect vp)
{
    const auto coord_step = (b - a) / (static_cast<float>(n) - 1.f);
    const auto value_step = (end - start) / (n - 1.0);

    for(auto i = 0; i < n; i++)
    {
        auto p = glm::project(a, glm::mat4(1.f), mvp, glm::vec4 {vp.x, vp.y, vp.w, vp. h});
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
    trenderer("../../../res/fonts/Quivira.otf", 13)
{
    assert(n > 0);
    bottom = grid({-1,-1,-1}, {-1,-1, 1}, { 1,-1,-1}, { 1,-1, 1}, n);
    up     = grid({ 1, 1,-1}, { 1, 1, 1}, {-1, 1,-1}, {-1, 1, 1}, n);
    left   = grid({-1, 1, 1}, {-1,-1, 1}, {-1, 1,-1}, {-1,-1,-1}, n);
    right  = grid({ 1, 1,-1}, { 1,-1,-1}, { 1, 1, 1}, { 1,-1, 1}, n);
    front  = grid({ 1,-1,-1}, { 1, 1,-1}, {-1,-1,-1}, {-1, 1,-1}, n);
    back   = grid({-1,-1, 1}, {-1, 1, 1}, { 1,-1, 1}, { 1, 1, 1}, n);

    orthogonal_proj = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
}

auto draw_grid(grid& g, glm::mat4 mvpersp, glm::mat3 mvortho) -> bool
{
    auto a = mvortho * g.b - mvortho * g.a;
    auto b = mvortho * g.c - mvortho * g.a;

    auto mat = glm::mat3(a, b, glm::vec3(1));
    // manual face culling: it is better to have mvp constructed from orthographic
    // projection matrix to avoid two parallel faces being displayed at the same time
    if(glm::determinant(mat) > 0)
    {
        g.draw(mvpersp);
        return true;
    }
    return false;
}


auto mona::axes3::draw(const camera& cam, mona::targets::target& t) -> void
{
    t.begin_frame();

    auto vp = t.viewport();
    auto perspective_proj =  glm::perspective(glm::radians(45.0f), vp.w/vp.h, 0.1f, 100.0f);

    auto mvporth = orthogonal_proj * cam.view();
    auto mvpersp = perspective_proj * cam.view();
    auto visible_bottom = draw_grid(bottom, mvpersp, mvporth);
    auto visible_up     = draw_grid(up, mvpersp, mvporth);
    auto visible_left   = draw_grid(left, mvpersp, mvporth);
    auto visible_right  = draw_grid(right, mvpersp, mvporth);
    auto visible_front  = draw_grid(front, mvpersp, mvporth);
    auto visible_back   = draw_grid(back, mvpersp, mvporth);

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


    auto ortho = glm::ortho(vp.x, vp.x + vp.w, vp.y, vp.y + vp.h);

    for(auto& mesh: meshes)
    {
        mesh.draw(mvpersp);
    }

    trenderer.s.set_uniform("projection", ortho);
    linspace(trenderer, x1, x2, x.x, x.y, n, font_origin_x, mvpersp, vp);
    linspace(trenderer, y1, y2, y.x, y.y, n, font_origin_y, mvpersp, vp);
    linspace(trenderer, z1, z2, z.x, z.y, n, font_origin_z, mvpersp, vp);

    t.end_frame();
    meshes.clear();
}

auto mona::axes3::submit(const mona::surface_mesh& mesh) -> void
{
    meshes.push_back(mesh);
}

