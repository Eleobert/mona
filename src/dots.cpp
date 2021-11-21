#include "mona/dots.hpp"

#include "slam/slam.h"

#include <algorithm>


using namespace gl;


struct vertex
{
    glm::vec3 pos;
    glm::vec4 color;
};


auto get_vertices(const arma::fvec& x, const arma::fvec& y, const std::vector<glm::vec4>& colors)
{
    std::vector<vertex> vertices;
    vertices.reserve(x.size());
    std::random_device rd;
    std::mt19937 gen(rd());
    //TODO: fix this hack
    std::uniform_real_distribution<> dis(0.f, 0.001f);
    double f = dis(gen);

    for(auto i = 0; i < x.size(); i++)
    {
        vertices.emplace_back(vertex{glm::vec3(x[i], y[i], f+=0.001), colors[i] / 255.f});
    }
    return vertices;
}


auto gen_buffer(const arma::fvec& x, const arma::fvec& y,
                const arma::fvec& z, const std::vector<glm::vec4>& colors) -> std::tuple<uint, uint>
{
    static auto vertices = get_vertices(x, y, colors);
    uint vao, vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo );
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertex), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)sizeof(glm::vec3));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return {vao, vbo};
}


mona::dots::dots(const arma::fvec& x, const arma::fvec& y, glm::vec4 color): size(x.size()),
    s("../../../res/shaders/lines.vert", "../../../res/shaders/dots.frag",
      "../../../res/shaders/dots.geom"), span_area{x.min(), y.min(), x.max(), y.max()}
{
    assert(x.size() == y.size());
    auto colors = std::vector<glm::vec4>(x.size());
    std::ranges::fill(colors, color);
    std::tie(vao, vbo) = gen_buffer(x, y, arma::fvec(x.size(), arma::fill::zeros), colors);
    size = std::ranges::ssize(x);
}


mona::dots::dots(const arma::fvec& x, const arma::fvec& y, const std::vector<glm::vec4>& colors):
    size(x.size()), s("../../../res/shaders/lines.vert", "../../../res/shaders/dots.frag",
                      "../../../res/shaders/dots.geom"), span_area{x.min(), y.min(), x.max(),
                                                                   y.max()}
{
    assert(x.size() == y.size());
    assert(x.size() == colors.size());
    span_area = {x.min(), y.min(), x.max(), y.max()};
    std::tie(vao, vbo) = gen_buffer(x, y, arma::fvec(x.size(), arma::fill::zeros), colors);
    size = std::ranges::ssize(x);
}


auto mona::dots::draw(const glm::mat4& mvp) const -> void
{
    int vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    s.use();
    s.set_uniform("vp_size", glm::ivec2(vp[2], vp[3]));
    s.set_uniform("radius", radius);

    s.set_uniform("mvp", mvp);
    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, size);
    glBindVertexArray(0);
    glLineWidth(1.f);
    s.unuse();
}
