#pragma once

#include <mona/rect.hpp>

#include "line_renderer.hpp"
#include "colors.hpp"

#include <armadillo>


namespace mona
{
    class line
    {
        line_renderer renderer;
        mona::rect span_area;

    public:
        glm::vec4 color;
        const arma::fvec x, y, z;
        line(const arma::fvec& x, const arma::fvec& y, const arma::fvec& z,
             glm::vec4 color = colors::black);
        line(const arma::fvec& x, const arma::fvec& y,
             glm::vec4 color = colors::black);

        // returns the area that this series cover
        // this is to be used to calculate the axes tick values
        auto span() const
        {
            return span_area;
        }

        auto draw(const glm::mat4& mvp)
        {
            renderer.draw(mvp, color, 1.5f);
        }

        auto reset(const arma::fvec& x, const arma::fvec y) -> void;

    };
};
