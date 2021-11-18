#include "mona/line.hpp"

auto submit(line_renderer& renderer, const arma::fvec& x, const arma::fvec& y, const arma::fvec& z)
{
    mona::rect span_area{x.min(), y.min(), x.max(), y.max()};

    for(auto i = 1; i < x.size(); i++)
    {
        // TODO: improve line renderer class
        renderer.submit({x[i - 1], y[i - 1], z[i - 1]}, {x[i], y[i], z[i]});
    }
    renderer.gen_buffer();

    return span_area;
}


mona::line::line(const arma::fvec& x, const arma::fvec& y, const arma::fvec& z, glm::vec4 color):
    x(x), y(y), z(z), color(color)
{
    span_area = submit(renderer, x, y, z);
}


mona::line::line(const arma::fvec& x, const arma::fvec& y, glm::vec4 color): x(x), y(y),
    z(x.size(),arma::fill::zeros), color(color)
{
    span_area = submit(renderer, x, y, z);
}

auto mona::line::reset(const arma::fvec& x, const arma::fvec y) -> void
{
    assert(x.size() > 0);
    assert(x.size() == y.size());
    // TODO: this is not good, too much data copy
    auto vec = std::vector<line_renderer::vertex>(x.size() - 1);

    for(auto i = 1; i < vec.size(); i++)
    {
        vec[i].a.x = x[i - 1];
        vec[i].a.y = y[i - 1];

        vec[i].b.x = x[i];
        vec[i].b.y = y[i];
    }
    renderer.reset(vec);
    span_area = mona::rect{x.min(), y.min(), x.max(), y.max()};
}


