#include <armadillo>

#include <mona/utility.hpp>
#include <mona/line.hpp>
#include <mona/targets/window.hpp>
#include <mona/axes.hpp>
#include <mona/colors.hpp>


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


int main()
{
    arma::fvec x = mona::linspace(-4, 4, 15);
    arma::fvec y = random(x, 0, 0.9);

    auto target = mona::targets::window();
    auto cam = mona::camera();
    auto axes = mona::axes();

    auto dots = mona::dots(x, y, mona::colors::tomato);
    auto line = mona::line(x, fit(x, y), mona::colors::tomato);

    float delta = 0;
    while (target.active())
    {
        axes.submit(dots);
        axes.submit(line);

        cam = target.control_camera(cam);
        target.submit(axes);
        target.draw();
    }
}