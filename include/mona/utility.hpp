#include <armadillo>
#include <cassert>

namespace mona
{
    auto meshgrid(const arma::vec& x, const arma::vec& y) -> std::tuple<arma::mat, arma::mat>
    {
        arma::mat x_mat(y.size(), x.size());
        arma::mat y_mat(y.size(), x.size());

        x_mat.each_row() = x.t();
        y_mat.each_col() = y;

        return {x_mat, y_mat};
    }

    template <typename Callable>
    auto apply(Callable f, const arma::mat& x, const arma::mat& y) -> arma::mat
    {
        assert(x.n_rows == y.n_rows);
        assert(x.n_cols == y.n_cols);

        arma::mat z(x.n_rows, x.n_cols);

        for(auto j = 0ul; j < x.n_cols; j++)
        {
            for(auto i = 0ul; i < x.n_rows; i++)
            {
                z(i , j) = f(x(i, j), y(i, j));
            }
        }
        return z;
    }


    auto linspace(double a, double b, size_t n)
    {
        assert(b > a);
        assert(n > 1);

        arma::vec res(n);
        const auto step = (b - a) / (n - 1);
        auto val = a;
        for(auto& e: res)
        {
            e = val;
            val += step;
        }
        // make the last value match b exactly
        res[n - 1] = b;
        return res;
    }
}