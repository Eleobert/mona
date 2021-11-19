#pragma once

#include <mona/rect.hpp>

#include "line_renderer.hpp"
#include "colors.hpp"
#include "mona/internal/shader.hpp"

#include <armadillo>



namespace mona
{
    class dots
    {
    private:

        mona::rect span_area;
        uint vbo = 0, vao = 0;
        size_t size = 0;
        shader s;
        std::vector<glm::vec3> vertices;

    public:
//        line(const arma::fvec& x, const arma::fvec& y, const arma::fvec& z,
//             glm::vec4 color = colors::black);
        dots(const arma::fvec& x, const arma::fvec& y);

        // returns the area that this series cover
        // this is to be used to calculate the axes tick values
        auto span() const
        {
            return span_area;
        }

        //auto reset(const arma::fvec& x, const arma::fvec y) -> void;
        auto draw(const glm::mat4& mvp) -> void;
        auto gen_buffer() -> void;


    };
};

