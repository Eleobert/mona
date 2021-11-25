#include "../slam/slam.h"

#include "mona/targets/window.hpp"
#include "mona/surface_mesh.hpp"
#include "mona/axes3.hpp"

#include <armadillo>


namespace mona::plot
{
    auto surface(const arma::fmat& x, const arma::fmat& y, const arma::fmat& z) -> void
    {
        auto win = mona::targets::window();
        auto cam  = mona::camera();
        auto mesh = mona::surface_mesh(x, y, z);
        auto axes = mona::axes3({x.min(), x.max()}, {y.min(), y.max()}, {z.min(), z.max()}, 5);
        //axes.set_camera_control(win.camera_control());

        while (win.active())
        {
            axes.submit(mesh);
            win.submit(axes);
            win.draw();
        }
    }
};