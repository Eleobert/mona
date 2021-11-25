#pragma once

#include "mona/line_grid.hpp"
#include "mona/source.hpp"
#include "mona/text_renderer.hpp"
#include "mona/camera.hpp"
#include "mona/targets/target.hpp"
#include "mona//surface_mesh.hpp"
#include <mona/camera_input_control.hpp>

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

#include <vector>
#include <memory>


namespace mona
{
    class axes3: public source
    {
        mutable camera cam;
        glm::mat4 perspective_proj;
        glm::mat4 orthogonal_proj;

        glm::vec2 x, y, z;
        int n;
        // TODO: fix mutable
        mutable line_grid bottom;
        mutable line_grid up;
        mutable line_grid left;
        mutable line_grid right;
        mutable line_grid front;
        mutable line_grid back;

        mutable std::vector<const mona::surface_mesh*> meshes;
        mutable std::unique_ptr<mona::camera_control> cam_control = nullptr;
    public:

        mutable text_renderer trenderer;

        axes3(glm::vec2 x, glm::vec2 y, glm::vec2 z, int n);
        auto draw(mona::rect area) const -> void;
        auto submit(const mona::surface_mesh& mesh) -> void;

        template<typename Control>
        auto set_camera_control(const Control& control)
        {
            cam_control = std::make_unique<Control>(control);
        }
    };
}