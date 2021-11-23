#include <mona/targets/target.hpp>
#include <mona/rect.hpp>


namespace mona::targets
{
    class viewport: public target
    {
        mona::rect vp_area{0, 0, 0, 0};
    public:
        viewport() = default;
        explicit viewport(mona::rect rect): vp_area(rect)
        {

        }

        mona::rect area() override
        {
            return vp_area;
        }

        void begin_frame() override;
        void end_frame() override;
        bool active() override;
        void draw() override
        { };
    };
}
