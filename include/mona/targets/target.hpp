#pragma once

#include "mona/camera.hpp"
#include <mona/rect.hpp>

namespace mona::targets
{
    class target
    {
    public:
        /**
         * Some targets may want to control the camera, e.g:
         * a window target with a 3d plot.
         */
        virtual auto control_camera(mona::camera camera) -> mona::camera
        {
            return camera;
        }
        
        virtual auto begin_frame() -> void = 0;
        
        virtual auto end_frame() -> void = 0;

        virtual auto active() -> bool = 0;

        virtual auto draw() -> void = 0;

        virtual auto area() -> mona::rect = 0;
    };
}