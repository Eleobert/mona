#pragma once

//#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <tuple>
#include <iostream>

namespace mona
{
    class camera
    {
        float distance = 5.f, pitch = 45.f, yaw = 45.f;

    public:

        camera() = default;

        auto view() const -> glm::mat4;

        auto zoom(float z)
        {
            distance += z;
        }

        auto rotate(float yaw, float pitch)
        {
            pitch = std::abs(this->pitch + pitch) < 89.f? pitch: 0;
            this->pitch += pitch;
            this->yaw += yaw;
        }

        auto pos() -> std::tuple<float, float, float>
        {
            return {distance, yaw, pitch};
        }

    };
}