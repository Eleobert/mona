#pragma once

#include <mona/resource.hpp>
#include <mona/rect.hpp>

#include <glm/matrix.hpp>



namespace mona
{
    class source: public resource
    {
    public:
        virtual auto draw(mona::rect area) const -> void = 0;
    };

    inline const source* exchange(const source* take, const source* release)
    {
        if(release)
        {
            release->release();
        }

        if(take)
        {
            take->hold();
        }
        return take;
    }
}