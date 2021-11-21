#include "mona/line.hpp"

#include <limits>


auto submit(line_renderer& renderer, const arma::fvec& x, const arma::fvec& y, const arma::fvec& z)
{
    mona::rect span_area{x.min(), y.min(), x.max(), y.max()};

    for(auto i = 0; i < x.size(); i++)
    {
        // TODO: improve line renderer class
        renderer.submit({x[i], y[i], z[i]});
    }
    renderer.gen_buffer();

    return span_area;
}


mona::line::line(const arma::fvec& x, const arma::fvec& y, const arma::fvec& z, glm::vec4 color):
    color(color)
{
    span_area = submit(renderer, x, y, z);
}


mona::line::line(const arma::fvec& x, const arma::fvec& y, glm::vec4 color): color(color)
{
    span_area = submit(renderer, x, y, arma::fvec(x.size(),arma::fill::zeros));
}


constexpr auto fnan = std::numeric_limits<float>::quiet_NaN();

mona::line::line(): span_area{fnan, fnan, fnan, fnan}
{

}

auto mona::line::reset(const arma::fvec& x, const arma::fvec& y) -> void
{
    assert(!x.empty());
    assert(x.size() == y.size());
    // TODO: this is not good, too much data copy
    auto vec = std::vector<glm::vec3>(x.size());

    for(auto i = 0; i < vec.size(); i++)
    {
        vec[i].x = x[i];
        vec[i].y = y[i];
        vec[i].z = 0;
    }
    renderer.reset(vec);
    span_area = mona::rect{x.min(), y.min(), x.max(), y.max()};
}


auto mona::line::empty() -> bool
{
    return not renderer.size();
}


