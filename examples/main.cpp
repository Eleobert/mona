#include <armadillo>

#include <mona/utility.hpp>
#include <mona/plot/surface.hpp>

auto f(double x, double y)
{
    return (7 * x * y) / std::exp(x*x + y*y);
}

int main()
{
    arma::vec line = mona::linspace(-2, 2, 50);
    auto [x, y] = mona::meshgrid(line, line);

    auto z = mona::apply(f, x, y);

    mona::plot::surface(x, y, z);
}