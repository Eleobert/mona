#pragma once

#include <mona/rect.hpp>

#include <glm/matrix.hpp>


namespace mona
{
    class source
    {
        mutable int n_targets = 0;
    public:
        virtual auto draw(mona::rect area) const -> void = 0;
        void hold() const{n_targets++;}
        void release() const
        {
            assert(n_targets > 0);
            n_targets--;
        }
        ~source()
        {
            // check if there is any target currently holding this source
            assert(n_targets == 0);
        }
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