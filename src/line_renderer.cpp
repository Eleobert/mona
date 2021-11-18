#include "slam/slam.h"
#include "mona/line_renderer.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstring>

using namespace gl;

line_renderer::line_renderer():
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
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertex), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    size = vertices.size();
}


void line_renderer::draw(const glm::mat4& mvp, glm::vec4 color, float thickness)
{
    glLineWidth(thickness);
    s.use();
    s.set_uniform("color", color / 255.f);
    s.set_uniform("mvp", mvp);
    glBindVertexArray(vao);
    glDrawArrays(gl::GL_LINES, 0, vertices.size()*4);
    glBindVertexArray(0);
    glLineWidth(1.f);
    s.unuse();
}


auto line_renderer::reset(const std::vector<vertex>& vertices) -> void
{
    // TODO: allow resizing
    assert(vertices.size() == size);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    auto* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    if (!ptr)
    {
        std::clog << "Error: cannot get VBO: " << vbo << "\n";
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        std::exit(0);
    }
    std::memcpy(ptr, vertices.data(), vertices.size()*sizeof(vertex));
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}