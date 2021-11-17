#include "../slam/slam.h"

#include "mona/targets/window.hpp"
#include "mona/surface_mesh.hpp"
#include "mona/axes3.hpp"

#include <armadillo>


namespace mona::plot
{
    auto surface(const arma::fmat& x, const arma::fmat& y, const arma::fmat& z) -> void
    {
        auto target = mona::targets::window();
        auto [w, h] = target.size();
        auto cam  = mona::camera(w, h);
        auto mesh = mona::surface_mesh(x, y, z);
        auto axes = mona::axes3({x.min(), x.max()}, {y.min(), y.max()}, {z.min(), z.max()}, 5);
        // auto picker = mona::picker(mesh);

        while (target.active())
        {
            cam = target.control_camera(cam);
            mesh.draw(cam, target);
            axes.draw(cam, target);
            target.draw();
        }
    }
};