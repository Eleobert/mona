#include <armadillo>

#include <mona/utility.hpp>
#include <mona/line.hpp> // should be called line instead
#include <mona/targets/window.hpp>
#include <mona/axes.hpp>
#include <mona/colors.hpp>


auto fit(const arma::fvec& x, const arma::fvec& y) -> std::tuple<float, float>
{
    auto xx = x - arma::mean(x);
    auto yy = y - arma::mean(y);
    float b = arma::sum(xx % yy) / arma::sum(xx % xx);
    float a = arma::mean(y) - b * arma::mean(x);
    return {a, b};
}


int main()
{
    arma::fvec x = mona::linspace(-4, 4, 15);
    arma::fvec y = x;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> dis(0, 0.9);

    for(auto& e: y)
    {
        e += dis(gen);
    }

    auto [a, b] = fit(x, y);
    arma::fvec reg = a + b * x;

    auto target = mona::targets::window();
    auto vp = target.viewport();
    auto cam = mona::camera();
    auto axes = mona::axes();

    auto dots = mona::dots(x, y);
    auto line = mona::line(x, reg, mona::colors::tomato);

    float delta = 0;
    while (target.active())
    {
        axes.submit(dots);
        axes.submit(line);

        cam = target.control_camera(cam);
        axes.draw(cam, target); // we should not need camera here
        target.draw();
    }
}