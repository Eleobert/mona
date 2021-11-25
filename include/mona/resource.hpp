#pragma once

#include <mona/rect.hpp>

#include <glm/matrix.hpp>


namespace mona
{
    /**
     *  After submitting, the target holds a pointer to the source. To make this
     *  process safe every source must to inherit from this class, and the targets
     *  must to call the member function hold or the function exchange, to assert
     *  if an attempt to destroy the source while it is being hold by a target is made.
     */
    class resource
    {
        mutable int counter = 0;
    public:

        void hold() const
        {
            counter++;
        }

        void release() const
        {
            assert(counter > 0);
            counter--;
        }

        ~resource()
        {
            // assert that there are no targets currently holding this source
            assert(counter == 0);
        }
    };
}