#pragma once

//#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <tuple>

namespace mona
{
    class camera
    {
        float distance = 5.f, pitch = 0, yaw = 90.f;

    public:

        camera() = default;

        auto view() const -> glm::mat4;

        auto zoom(float z)
        {
            distance += z;
        }

        auto rotate(float yaw, float pitch)
        {
            this->yaw += yaw;
            this->pitch += pitch;
        }

        auto pos() -> std::tuple<float, float, float>
        {
            return {distance, yaw, pitch};
        }

    };
}