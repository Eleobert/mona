#pragma once

#include "mona/line_grid.hpp"
#include "mona/text_renderer.hpp"
#include "mona/camera.hpp"
#include "mona/targets/target.hpp"
#include "mona//surface_mesh.hpp"

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

#include <vector>


namespace mona
{
    class axes3
    {
        glm::mat4 perspective_proj;
        glm::mat4 orthogonal_proj;

        glm::vec2 x, y, z;
        int n;
        line_grid bottom;
        line_grid up;
        line_grid left;
        line_grid right;
        line_grid front;
        line_grid back;

        std::vector<mona::surface_mesh> meshes;
    public:

        text_renderer trenderer;

        axes3(glm::vec2 x, glm::vec2 y, glm::vec2 z, int n);
        auto draw(const camera& cam, mona::targets::target& t) -> void;
        auto submit(const mona::surface_mesh& mesh) -> void;
    };
}