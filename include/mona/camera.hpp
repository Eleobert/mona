#pragma once

//#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <tuple>

namespace mona
{
    class camera
    {
        //glm::mat4 view;
        glm::mat4 perspective;

        float distance = 5.f, pitch = 0, yaw = 90.f;

    public:
        glm::vec4 view_port;
        glm::mat4 orthogonal;
        camera() = default;
        camera(float w, float h);
        auto mvprsp() const -> glm::mat4;
        auto mvorth() const -> glm::mat4;
        auto orth() const -> glm::mat4 {return orthogonal;}
        auto prsp() const -> glm::mat4 {return perspective;}
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