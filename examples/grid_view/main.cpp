#include <armadillo>

#include <mona/utility.hpp>
#include <mona/line.hpp>
#include <mona/targets/window.hpp>
#include <mona/axes.hpp>
#include <mona/colors.hpp>
#include <mona/grid.hpp>

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
    arma::fvec x = mona::linspace(-4, 4, 15);
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

int main()
{
    auto win  = mona::targets::window();
    auto view = mona::grid(2, 2);
    auto cam  = mona::camera();
    auto axes_dots = mona::axes();
    auto axes_line = mona::axes();

    auto dots = get_dots();
    auto line = get_line();

    while (win.active())
    {
        axes_dots.submit(dots);
        axes_line.submit(line);

        view(0, 0).submit(axes_dots);
        view(0, 1).submit(axes_line);
        view(1, 0).submit(axes_dots);
        view(1, 1).submit(axes_line);

        win.submit(view);
        win.draw();
    }
}