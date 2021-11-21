#include "slam/slam.h"
#include "mona/line_renderer.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstring>

using namespace gl;

line_renderer::line_renderer(): strip(GL_LINE_STRIP),
    s("../../../res/shaders/lines.vert", "../../../res/shaders/lines.frag")
{
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void line_renderer::gen_buffer()
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

    fixed_size = vertices.size();
}


void line_renderer::draw(const glm::mat4& mvp, glm::vec4 color, float thickness) const
{
    glLineWidth(thickness);
    s.use();
    s.set_uniform("color", color / 255.f);
    s.set_uniform("mvp", mvp);
    glBindVertexArray(vao);
    glDrawArrays(strip, 0, vertices.size());
    glBindVertexArray(0);
    glLineWidth(1.f);
    s.unuse();
}


auto line_renderer::reset(const std::vector<glm::vec3>& vertices) -> void
{
    // TODO: we don't  even need to store this
    this->vertices = vertices;
    // TODO: allow resizing
    assert(vertices.size() == fixed_size);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    auto* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    if (!ptr)
    {
        std::clog << "Error: cannot get VBO: " << vbo << "\n";
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        std::exit(0);
    }
    std::memcpy(ptr, vertices.data(), vertices.size()*sizeof(glm::vec3));
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


auto line_renderer::set_strip(bool strip) -> void
{
    this->strip = strip? GL_LINE_STRIP: GL_LINES;
}