#pragma once

#include "mona/camera.hpp"
#include "mona/targets/target.hpp"
#include "mona/internal/shader.hpp"
#include "mona/resource.hpp"

#include <armadillo>
#include <glm/vec3.hpp>

#include <vector>

namespace mona
{
    class surface_mesh: public resource
    {
        mutable shader s;
        unsigned int vao = 0;
        unsigned int vbo = 0;
        unsigned int ibo = 0;

    public:

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::uvec3> indices;

        surface_mesh(const arma::fmat& x, const arma::fmat& y, const arma::fmat& z);
        ~surface_mesh();

        auto draw(const glm::mat4& mvp) const -> void;
    };
}