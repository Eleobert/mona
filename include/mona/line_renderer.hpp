#pragma once

#include "mona/internal/shader.hpp"

#include <glm/vec3.hpp>
#include <vector>


class line_renderer
{
private:
    size_t fixed_size;
    shader s;
    std::vector<glm::vec3> vertices;
    uint32_t vao = 0;
    uint32_t vbo = 0;
    uint strip;
public:


    line_renderer();

    void submit(glm::vec3 a)
    {
        vertices.push_back(a);
    }

    auto gen_buffer() -> void;
    auto draw(const glm::mat4& mvp, glm::vec4 color, float thickness) -> void;
    auto reset(const std::vector<glm::vec3>& vertices) -> void;
    auto size() -> size_t {return vertices.size();}
    auto set_strip(bool strip) -> void;
};