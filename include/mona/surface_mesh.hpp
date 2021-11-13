#pragma once

#include "mona/camera.hpp"
#include "mona/targets/target.hpp"
#include "mona/internal/shader.hpp"

#include <armadillo>
#include <glm/vec3.hpp>

#include <vector>

namespace mona
{
    class surface_mesh
    {
        mutable shader s;
        unsigned int vao = 0;
        unsigned int vbo = 0;
        unsigned int ibo = 0;

    public:

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::uvec3> indices;

        surface_mesh(const arma::mat& x, const arma::mat& y, const arma::mat& z);
        ~surface_mesh();

        auto draw(const mona::camera& cam, mona::targets::target& t) const -> void;
    };
}