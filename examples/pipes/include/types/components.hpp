#pragma once
#include "entities/tile.hpp"
#include <SDL2/SDL.h>

namespace component {
struct tile {
    tiles::Name name;
    tiles::Rotation rotation;
};

struct position {
    int x, y;
};

struct static_tile {};
}
