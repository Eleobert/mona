#pragma once

#include <armadillo>

namespace mona::plot
{
    auto surface(const arma::fmat& x, const arma::fmat& y, const arma::fmat& z) -> void;
}