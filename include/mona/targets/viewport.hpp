#include <mona/targets/target.hpp>
#include <mona/rect.hpp>
#include <mona/source.hpp>

namespace mona::targets
{
    class viewport: public target, source
    {
        mona::rect vp_area{0, 0, 0, 0};
        mutable const mona::source* src = nullptr;
    public:
        viewport() = default;
        explicit viewport(mona::rect rect): vp_area(rect)
        {

        }

        mona::rect area() override
        {
            return vp_area;
        }

        auto submit(const mona::source& src)
        {
            this->src = mona::exchange(&src, this->src);
        }

        void begin_frame() override;
        void end_frame() override;
        bool active() override;
        auto draw(mona::rect r) const -> void override;
    };
}
