#include <armadillo>

#include <mona/utility.hpp>
#include <mona/plot/surface.hpp>
#include <mona/targets/window.hpp>
#include <mona/surface_mesh.hpp>
#include <mona/axes3.hpp>

auto f(double x, double y)
{
    return (7 * x * y) / std::exp(x*x + y*y);
}

int main()
{
    arma::fvec line = mona::linspace(-2, 2, 50);
    auto [x, y] = mona::meshgrid(line, line);
    auto z = mona::apply(f, x, y);

    auto win = mona::targets::window();
    auto mesh = mona::surface_mesh(x, y, z);
    auto axes = mona::axes3({x.min(), x.max()}, {y.min(), y.max()}, {z.min(), z.max()}, 5);
    axes.set_camera_control(win.get_camera_control());

    while (win.active())
    {
        axes.submit(mesh);
        win.submit(axes);
        win.draw();
    }
}