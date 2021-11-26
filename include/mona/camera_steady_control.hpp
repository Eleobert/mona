#pragma once

#include <mona/camera_control.hpp>


namespace mona
{
    class camera_steady_control: public camera_control
    {
    public:
        camera_steady_control(float d_yaw, float d_pitch): d_yaw(d_yaw), d_pitch(d_pitch)
        {}
        auto operator()(mona::rect area) -> control override
        {

            return control{0.f, d_yaw, d_pitch};
        }

    private:
        const float d_yaw   = 0.f;
        const float d_pitch = 0.f;
    };
}