#pragma once
#include <cstdint>
#include <cmath>
#include <SDL2/SDL_rect.h>

namespace component {

struct bbox {
    float x, y, size;

    // convert the bbox to an SDL_Rect
    explicit operator SDL_Rect() const noexcept
    {
        int const s = static_cast<int>(std::round(size));
        return SDL_Rect{static_cast<int>(std::round(x)),
                        static_cast<int>(std::round(y)), s, s};
    }
};

struct velocity {
    float x, y;
};

struct color {
    std::uint8_t r, g, b;
};

struct dynamic_mover {
    float acceleration;
    float friction;
    float minspeed;
    float maxspeed;
};

struct growth_rate {
    float value;
};

struct munchable {};
}
