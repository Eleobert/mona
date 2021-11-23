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
    int tmp[4];
    glGetIntegerv(GL_VIEWPORT, tmp);
    return tmp[0] == vp_area.x && tmp[1] == vp_area.y && tmp[2] == vp_area.w && tmp[3] == vp_area.h;
}
