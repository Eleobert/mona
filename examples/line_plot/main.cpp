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
    auto vp = target.viewport();
    auto cam = mona::camera(); // viewport should be set on control camera
    auto axes = mona::axes();

    auto line1 = mona::line(x, y, mona::colors::midnight_blue);
    auto line2 = mona::line(x, y % x, mona::colors::orange_red);
    auto line3 = mona::line(x, y - x, mona::colors::pale_violet_red);


    float delta = 0;
    while (target.active())
    {
        axes.submit(line1);
        axes.submit(line2);
        axes.submit(line3);

        arma::fvec t = x + delta;
        t.transform(f);
        delta += 0.04;

        line1.reset(x, t);
        line1.color.r = 255.f * std::cos(glfwGetTime());
        line1.color.b = 255.f * std::sin(glfwGetTime());
        line1.color.b = 255.f * std::sin(glfwGetTime()) * std::cos(glfwGetTime()) ;

        cam = target.control_camera(cam);
        axes.draw(cam, target); // we should not need camera here
        target.draw();
    }
}