#pragma once

#include <mona/rect.hpp>
#include <mona/targets/viewport.hpp>

#include <vector>


namespace mona
{
    class grid: public source
    {
        int m = 0, n = 0;
        std::vector<std::vector<targets::viewport>> ports;

    public:
        grid(int m, int n): m(m), n(n), ports(m)
        {
            for(auto i = 0; i < m; i++)
            {
                ports[i].resize(n);
            }
        }

        virtual auto draw(mona::rect area) const -> void
        {
            auto x_step = area.w / n;
            auto y_step = area.h / m;

            for(auto i = 0; i < m; i++)
            {
                for(int j = 0; j < n; j++)
                {
                    auto x = area.x + j * x_step;
                    auto y = area.y + (m - 1.f - i) * y_step;
                    // TODO: fix this
                    mona::rect a{x, y, area.w / n,  area.h / m};
                    ports[i][j].draw(a);
                }
            }
        }

        targets::viewport& operator()(int i, int j)
        {
            return ports[i][j];
        }


    };
}
