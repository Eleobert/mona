#pragma once

#include "mona/internal/shader.hpp"
#include "mona/rect.hpp"

#include <armadillo>
#include <glm/matrix.hpp>

namespace mona
{
    class bars
    {
        shader s;
        uint vao, vbo;
        int size = 0;
        mona::rect span_area;
    public:
        bars(const arma::fvec& x, const arma::fvec& height);
        bars(const arma::fvec& x, const arma::fvec& height, const arma::fvec& base);
        auto draw(const glm::mat4& mvp) -> void;

        // returns the area that this series cover
        // this is to be used to calculate the axes tick values
        auto span() const
        {
            return span_area;
        }
    };
}


