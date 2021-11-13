#pragma once

#include <glm/vec3.hpp>
#include <vector>


class line_renderer
{
    struct line
    {
        glm::vec3 a;
        glm::vec3 b;
    };
    
    std::vector<line> vertices;
    uint32_t vao = 0;
public:

    void submit(glm::vec3 a, glm::vec3 b)
    {
        vertices.push_back({a, b});
    }

    auto gen_buffer() -> void;
    auto draw() -> void;
};