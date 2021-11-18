#include "mona/camera.hpp"

#include <glm/gtc/matrix_transform.hpp>


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