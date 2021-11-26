#include <mona/utility.hpp>
#include <mona/targets/png.hpp>
#include <mona/axes3.hpp>
#include <mona/surface_mesh.hpp>

#include <armadillo>


auto f(double x, double y)
{
    return (2*x*x + y*y) * std::exp(1 - x*x -y*y);
};


template <typename F>
auto get_surface(F f)
{
    arma::fvec line = mona::linspace(-2, 2, 50);
    auto [x, y] = mona::meshgrid(line, line);
    auto z = mona::apply(f, x, y);

    auto mesh = mona::surface_mesh(x, y, z);

    return mesh;
}


int main()
{
    auto png  = mona::png(800, 600);
    auto axes = mona::axes3({-4, 4}, {-4, 4}, {-4, 4}, 5);
    auto surf = get_surface(f);

    axes.submit(surf);
    png.save(axes, "image.png");
}