#pragma once

#include "mona/internal/shader.hpp"

#include <glm/vec3.hpp>
#include <vector>


class line_renderer
{
    struct line
    {
        glm::vec3 a;
        glm::vec3 b;
    };

    shader s;
    std::vector<line> vertices;
    uint32_t vao = 0;
public:

    line_renderer();

    void submit(glm::vec3 a, glm::vec3 b)
    {
        vertices.push_back({a, b});
    }

    auto gen_buffer() -> void;
    auto draw(const glm::mat4& mvp, glm::vec4 color, float thickness) -> void;
};