#pragma once

#include "line_renderer.hpp"
#include "camera.hpp"

class grid
{

    int n;
    line_renderer renderer;

public:
    glm::vec3 a;
    glm::vec3 b;
    glm::vec3 c;
    glm::vec3 d;

    grid(): a(0), b(0), c(0), d(0){}

    grid(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, int n): a(a), b(b), c(c), d(d)
    {
        process(a, b, c, d, n);
        process(a, c, b, d, n);
        renderer.set_strip(false);
    }

    /**
     *   a ----- c
     *   |       |
     *   b-------d
     */ 
    void process(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, int n)
    {
        auto top_step = (c - a) / (n - 1.f);
        auto down_step = (d - b) / (n - 1.f);

        auto top = a;
        auto down = b;
        for(auto i = 0; i < n; i++)
        {
            renderer.submit(top);
            renderer.submit(down);
            top += top_step;
            down += down_step;
        }

        renderer.gen_buffer();
    }

    void draw(const glm::mat4& mvp)
    {
        renderer.draw(mvp, {0, 0, 0, 255}, 1.f);
    }
};