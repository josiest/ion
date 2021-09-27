#pragma once

#include "types/tile.hpp"
#include <SDL.h>

namespace component {
struct tile {
    tiles::Name name;
    tiles::Rotation rotation;
};

struct position {
    int x, y;
    inline operator SDL_Point() const { return {x, y}; }
};

struct static_tile {};
}
