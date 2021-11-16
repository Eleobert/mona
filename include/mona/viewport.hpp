#pragma once


namespace mona
{
    struct viewport
    {
        float x, y, w, h;
    };
};

auto get_current_view_port() -> mona::viewport;
