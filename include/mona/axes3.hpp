#pragma once

#include "mona/grid.hpp"
#include "mona/text_renderer.hpp"
#include "mona/camera.hpp"
#include "mona/targets/target.hpp"

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

namespace mona
{
    class axes3
    {
        glm::vec2 x, y, z;
        int n;
        grid bottom;
        grid up;
        grid left;
        grid right;
        grid front;
        grid back;
//        float series_padding = 0.05;
//        float panels_padding = 0.08

    public:

        text_renderer trenderer;

        axes3(glm::vec2 x, glm::vec2 y, glm::vec2 z, int n);
        auto draw(const camera& cam, mona::targets::target& t) -> void;
    };
}