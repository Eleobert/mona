#pragma once

#include <mona/rect.hpp>
#include <mona/resource.hpp>

#include <glm/matrix.hpp>

namespace mona
{
    class mesh: public resource
    {
    public:
        // returns the area that this series wants to be covered
        virtual auto span() const -> mona::rect = 0;
        virtual auto draw(const glm::mat4& mvp) const -> void = 0;
    };
};
