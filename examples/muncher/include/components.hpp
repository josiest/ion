#pragma once
#include <cstdint>
#include <cmath>
#include <SDL3/SDL_rect.h>

namespace component {

struct bbox {
    float x, y, size;

    // convert the bbox to an SDL_Rect
    explicit operator SDL_FRect() const noexcept
    {
        return { x, y, size, size };
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
    float min_speed;
    float max_speed;
};

struct growth_rate {
    float value;
};

struct munchable {};
}
