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
        int size = 0;
        shader s;
        std::vector<glm::vec3> vertices;
        float radius = 10;

    public:
        dots(const arma::fvec& x, const arma::fvec& y, const std::vector<glm::vec4>& colors);
        dots(const arma::fvec& x, const arma::fvec& y, glm::vec4 color = colors::black);

        // returns the area that this series cover
        // this is to be used to calculate the axes tick values
        auto span() const
        {
            return span_area;
        }

        //auto reset(const arma::fvec& x, const arma::fvec y) -> void;
        auto draw(const glm::mat4& mvp) -> void;

        auto set_radius(float r) -> void
        {
            radius = r;
        }
    };
};

