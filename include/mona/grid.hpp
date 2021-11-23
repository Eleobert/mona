#pragma once

#include <mona/rect.hpp>
#include <mona/targets/viewport.hpp>

#include <vector>


namespace mona
{
    class grid
    {
        int m = 0, n = 0;
        std::vector<std::vector<targets::viewport>> ports;

    public:
        grid(const mona::rect& area, int m, int n): m(0), n(0), ports(m)
        {
            auto x_step = area.w / n;
            auto y_step = area.h / m;

            for(auto i = 0; i < m; i++)
            {
                ports[i].resize(n);
                for(int j = 0; j < n; j++)
                {
                    mona::rect a{area.x + j * x_step, area.y + i * x_step, area.w / n,  area.h / n};
                    ports[i][j] = targets::viewport(a);
                }
            }
        }

        targets::viewport& operator()(int i, int j)
        {
            return ports[i][j];
        }
    };
}
