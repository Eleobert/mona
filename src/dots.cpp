#include "mona/dots.hpp"

#include "slam/slam.h"


using namespace gl;


auto submit(std::vector<glm::vec3>& vertices, const arma::fvec& x, const arma::fvec& y)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    //TODO: fix this hack
    std::uniform_real_distribution<> dis(0.f, 0.001f);
    double f = dis(gen);

    for(auto i = 0; i < x.size(); i++)
    {
        vertices.emplace_back(x[i], y[i], f+=0.001);
    }
}


auto mona::dots::gen_buffer() -> void
{

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo );
    glBindBuffer(GL_ARRAY_BUFFER, vbo );
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    size = vertices.size();
}


mona::dots::dots(const arma::fvec& x, const arma::fvec& y, glm::vec4 color): size(x.size()),
    s("../../../res/shaders/lines.vert", "../../../res/shaders/dots.frag",
      "../../../res/shaders/dots.geom"), color(color)
{
    span_area = {x.min(), y.min(), x.max(), y.max()};
    submit(vertices, x, y);
    gen_buffer();
}

auto mona::dots::draw(const glm::mat4& mvp) -> void
{
    int vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    s.use();
    s.set_uniform("vp_size", glm::ivec2(vp[2], vp[3]));
    s.set_uniform("radius", radius);
    s.set_uniform("color", color / 255.f);
    s.set_uniform("mvp", mvp);
    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, vertices.size()*4);
    glBindVertexArray(0);
    glLineWidth(1.f);
    s.unuse();
}
