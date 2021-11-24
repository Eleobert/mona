#pragma once

#include "mona/text_renderer.hpp"
#include "mona/line_renderer.hpp"
#include "mona/camera.hpp"
#include "mona/targets/target.hpp"
#include "mona/line.hpp"
#include "mona/internal/shader.hpp"
#include "mona/dots.hpp"
#include <mona/source.hpp>

#include <glm/vec2.hpp>
#include <vector>

namespace mona
{
    class axes: public source
    {
    public:
        struct params
        {
            params() { };
            float padding_up    = 0.15f;
            float padding_down  = 0.25f;
            float padding_left  = 0.25f;
            float padding_right = 0.15f;
            int n_bins = 6;
        };

    private:
        // TODO:draw should not be mutable
        axes::params par;
        mutable std::vector<const mesh*> ss;
        mona::rect prev_target_viewport;
        // TODO: or maybe draw should not be const
        mutable line port;
        mutable text_renderer trenderer;

    public:

        axes(params p = params());
        auto draw(mona::rect r) const -> void;
        auto submit(const mesh& s) -> void;

    };
}