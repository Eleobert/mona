#pragma once

#include "mona/rect.hpp"

namespace mona
{
    class camera_control
    {
    public:
        struct control
        {
            float zoom  = 0.f;
            float yaw   = 0.f;
            float pitch = 0.f;
        };

        virtual auto operator()(mona::rect area) -> control = 0;
    };
}