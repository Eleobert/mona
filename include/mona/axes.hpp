#pragma once

#include "mona/text_renderer.hpp"
#include "mona/line_renderer.hpp"
#include "mona/camera.hpp"
#include "mona/targets/target.hpp"
#include "mona/line.hpp"
#include "mona/internal/shader.hpp"
#include "mona/dots.hpp"

#include <glm/vec2.hpp>
#include <vector>

namespace mona
{
    class axes
    {
    public:
        struct params
        {
            params() { };
            float padding_up    = 0.25f;
            float padding_down  = 0.25f;
            float padding_left  = 0.30f;
            float padding_right = 0.25f;
            int n_bins = 6;
        };

    private:
        axes::params par;
        std::vector<line> ls;
        std::vector<mona::dots> ds;

    public:
        line_renderer port_boundary;
        text_renderer trenderer;

        axes(params p = params());
        auto draw(const camera& cam, mona::targets::target& t) -> void;
        auto submit(const line& ls) -> void;
        auto submit(const dots& ls) -> void;

    };
}