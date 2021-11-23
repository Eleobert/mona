#include <mona/targets/viewport.hpp>

#include "../slam/slam.h"


using namespace mona::targets;

auto viewport::begin_frame() -> void
{
    glViewport(vp_area.x, vp_area.y, vp_area.w, vp_area.h);
}

auto viewport::end_frame() -> void
{

}

bool viewport::active()
{
    return true;
//    int tmp[4];
//    glGetIntegerv(GL_VIEWPORT, tmp);
//    return tmp[0] == vp_area.x && tmp[1] == vp_area.y && tmp[2] == vp_area.w && tmp[3] == vp_area.h;
}

auto viewport::draw(mona::rect r) const -> void
{
    int tmp[4];
    glGetIntegerv(GL_VIEWPORT, tmp);

    glViewport(r.x, r.y, r.w, r.h);

    if(src)
    {
        src->draw(r);
    }
    src = mona::exchange(nullptr, src);
    glViewport(tmp[0], tmp[1], tmp[2], tmp[3]);
};