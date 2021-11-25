#include <mona/utility.hpp>
#include <mona/line.hpp>
#include <mona/targets/window.hpp>
#include <mona/axes.hpp>
#include <mona/axes3.hpp>
#include <mona/colors.hpp>
#include <mona/grid.hpp>
#include <mona/surface_mesh.hpp>
#include <mona/camera_rotation_control.hpp>

#include <armadillo>


auto fit(const arma::fvec& x, const arma::fvec& y) -> arma::fvec
{
    auto xx = x - arma::mean(x);
    auto yy = y - arma::mean(y);
    float b = arma::sum(xx % yy) / arma::sum(xx % xx);
    float a = arma::mean(y) - b * arma::mean(x);
    return a + b * x;
}


auto random(arma::fvec x, float mean, float std)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> dis(mean, std);

    for(auto& e: x)
    {
        e += dis(gen);
    }

    return x;
}


auto get_dots()
{
    arma::fvec x = mona::linspace(-4, 4, 30);
    arma::fvec y = random(x, 0, 0.9);
    return mona::dots(x, y, mona::colors::tomato);
}

auto get_line()
{
    auto f = [](double x)
    {
        return std::sin(x);
    };

    arma::fvec x = mona::linspace(-4, 4, 50);
    arma::fvec y = x;
    y.transform(f);

    return mona::line(x, y, mona::colors::midnight_blue);
}


auto f1(double x, double y)
{
    return (7 * x * y) / std::exp(x*x + y*y);
};

auto f2(double x, double y)
{
    return std::pow(x, 4) + std::pow(y, 4) - std::pow(x + y, 2);
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
    auto win  = mona::targets::window();
    auto view = mona::grid(2, 2);
    auto cam  = mona::camera();
    auto axes_dots = mona::axes();
    auto axes_line = mona::axes();
    auto axes_surf1 = mona::axes3({-4, 4}, {-4, 4}, {-4, 4}, 5);
    axes_surf1.set_camera_control(win.get_camera_control());

    auto axes_surf2 = mona::axes3({-4, 4}, {-4, 4}, {-4, 4}, 5);
    axes_surf2.set_camera_control(mona::camera_rotation_control(0.2f, 0.f));

    auto dots = get_dots();
    auto line = get_line();
    auto surf1 = get_surface(f1);
    auto surf2 = get_surface(f2);

    while (win.active())
    {
        axes_dots.submit(dots);
        axes_line.submit(line);
        axes_surf1.submit(surf1);
        axes_surf2.submit(surf2);

        view(0, 0).submit(axes_dots);
        view(0, 1).submit(axes_surf1);
        view(1, 0).submit(axes_surf2);
        view(1, 1).submit(axes_line);

        win.submit(view);
        win.draw();
    }
}