#include "slam/slam.h"
#include "mona/line_renderer.hpp"

using namespace gl;

void line_renderer::gen_buffer()
{

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo );
    glBindBuffer(GL_ARRAY_BUFFER, vbo );
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(line), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void line_renderer::draw(const glm::mat4& mvp, glm::vec4 color)
{
    s.use();
    s.set_uniform("color", color / 255.f);
    s.set_uniform("mvp", mvp);
    glBindVertexArray(vao);
    glDrawArrays(gl::GL_LINES, 0, vertices.size()*4);
    glBindVertexArray(0);

}