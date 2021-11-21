#pragma once


namespace mona
{
    struct rect
    {
        float x, y, w, h;
        auto operator==(const rect&) const -> bool = default;
    };
};