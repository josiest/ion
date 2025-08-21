#pragma once
#include <SDL2/SDL_rect.h>

namespace component {
struct position {
    int x, y;
    inline operator SDL_Point() const { return {x, y}; }
};

struct static_tile {};
}
