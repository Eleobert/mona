#include "mona/camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

mona::camera::camera(float w, float h)
{
    perspective = glm::perspective(glm::radians(45.0f), w/h, 0.1f, 100.0f);
    orthogonal = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
}


auto mona::camera::view() const -> glm::mat4
{
    auto pos = glm::vec3(0.f);
    pos.x = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
    pos.y = glm::sin(glm::radians(pitch));
    pos.z = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
    pos *= distance;

    auto front = glm::normalize(-pos);
    auto right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    auto up    = glm::normalize(glm::cross(right, front));

    return glm::lookAt(pos, glm::vec3(0.f), up);
}


auto mona::camera::mvprsp() const -> glm::mat4
{
    return perspective * view();
}


auto mona::camera::mvorth() const -> glm::mat4
{
    return orthogonal * view();
}