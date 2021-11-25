#pragma once


namespace mona
{
    struct rect
    {
        float x, y, w, h;
        auto operator==(const rect&) const -> bool = default;
    };

    template<typename Point>
    bool is_in(const Point& p, rect r)
    {
        return p.x > r.x && p.x < (r.x + r.w) && p.y > r.y && p.y < (r.y + r.w);
    }
};