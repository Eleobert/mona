#pragma once

#include "line_renderer.hpp"
#include "colors.hpp"

#include <armadillo>


namespace mona
{
    class line
    {
        line_renderer renderer;
        glm::vec4 color;

    public:
        const arma::fvec x, y, z;
        line(const arma::fvec& x, const arma::fvec& y, const arma::fvec& z,
             glm::vec4 color = colors::black);
        line(const arma::fvec& x, const arma::fvec& y,
             glm::vec4 color = colors::black);

        auto draw(const glm::mat4& mvp)
        {
            renderer.draw(mvp, color);
        }

    };
};
