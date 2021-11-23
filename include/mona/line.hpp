#pragma once

#include <mona/rect.hpp>
#include <mona/series.hpp>

#include "line_renderer.hpp"
#include "colors.hpp"

#include <armadillo>


namespace mona
{
    class line: public series
    {
        line_renderer renderer;
        mona::rect span_area;

    public:
        glm::vec4 color{};

        line(const arma::fvec& x, const arma::fvec& y, const arma::fvec& z,
             glm::vec4 color = colors::black);
        line(const arma::fvec& x, const arma::fvec& y,
             glm::vec4 color = colors::black);
        line();

        // returns the area that this series cover
        // this is to be used to calculate the axes tick values
        auto span() const -> mona::rect override
        {
            return span_area;
        }

        auto draw(const glm::mat4& mvp) const -> void override
        {
            renderer.draw(mvp, color, 1.5f);
        }

        auto reset(const arma::fvec& x, const arma::fvec& y) -> void;

        // TODO: I am not sure if this is to stay
        auto empty() const -> bool;

        auto set_strip(bool strip) -> void
        {
            renderer.set_strip(strip);
        }
    };
};
