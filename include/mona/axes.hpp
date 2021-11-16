#pragma once

#include "mona/text_renderer.hpp"
#include "mona/line_renderer.hpp"
#include "mona/camera.hpp"
#include "mona/targets/target.hpp"
#include "mona/line.hpp"
#include "mona/internal/shader.hpp"

#include <glm/vec2.hpp>
#include <vector>

namespace mona
{
    class axes
    {
        std::vector<line> ls;
        glm::vec2 x, y;
        glm::mat4 ortho;
        glm::vec3 bg_color;

    public:
        line_renderer lrenderer;
        //text_renderer trenderer;

        axes();
        auto draw(const camera& cam, mona::targets::target& t) -> void;
        auto add(const line& ls) -> void;

    };
}