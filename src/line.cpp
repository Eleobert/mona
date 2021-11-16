#include "mona/line.hpp"

auto submit(line_renderer& renderer, const arma::fvec& x, const arma::fvec& y, const arma::fvec& z) -> void
{
    for(auto i = 1; i < x.size(); i++)
    {
        // TODO: improve line renderer class
        renderer.submit({x[i - 1], y[i - 1], z[i - 1]}, {x[i], y[i], z[i]});
    }
    renderer.gen_buffer();
}


mona::line::line(const arma::fvec& x, const arma::fvec& y, const arma::fvec& z, glm::vec4 color):
    x(x), y(y), z(z), color(color)
{
    submit(renderer, x, y, z);
}


mona::line::line(const arma::fvec& x, const arma::fvec& y, glm::vec4 color): x(x), y(y),
    z(x.size(),arma::fill::zeros), color(color)
{
    submit(renderer, x, y, z);
}

