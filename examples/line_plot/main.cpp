#include <armadillo>

#include <mona/utility.hpp>
#include <mona/line.hpp> // should be called line instead
#include <mona/targets/window.hpp>
#include <mona/axes.hpp>
#include <mona/colors.hpp>

auto f(double x)
{
    return std::sin(x);
}

int main()
{
    arma::fvec x = mona::linspace(-4, 4, 50);
    arma::fvec y = x;
    y.transform(f);

    auto target = mona::targets::window();
    auto [w, h] = target.size();
    auto cam = mona::camera(w, h); // viewport should be set on control camera
    auto axes = mona::axes();

    auto line1 = mona::line(x, y, mona::colors::midnight_blue);
    auto line2 = mona::line(x, y % x, mona::colors::orange_red);
    auto line3 = mona::line(x, y - x, mona::colors::pale_violet_red);

    axes.add(line1);
    axes.add(line2);
    axes.add(line3);

    float f = 0.0;
    while (target.active())
    {
        cam = target.control_camera(cam);
        axes.draw(cam, target); // we sould not need camera here
        target.draw();
    }
}