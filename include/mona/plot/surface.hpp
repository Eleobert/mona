#pragma once

#include <armadillo>

namespace mona::plot
{
    auto surface(const arma::mat& x, const arma::mat& y, const arma::mat& z) -> void;
}