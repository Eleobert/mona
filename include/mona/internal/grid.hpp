#pragma once

#include "line_renderer.hpp"


class line_grid
{

    int n;
    line_renderer renderer;

public:
    glm::vec3 a;
    glm::vec3 b;
    glm::vec3 c;
    glm::vec3 d;

    line_grid(): a(0), b(0), c(0), d(0){}

    line_grid(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, int n): a(a), b(b), c(c), d(d)
    {
        process(a, b, c, d, n);
        process(a, c, b, d, n);
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
            renderer.submit(top, down);
            top += top_step;
            down += down_step;
        }

        renderer.gen_buffer();
    }

    void draw()
    {
        renderer.draw();
    }
};