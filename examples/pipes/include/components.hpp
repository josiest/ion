#pragma once
#include "entities/tile.hpp"
#include <SDL2/SDL.h>

struct Tile {
    tiles::Name name;
    tiles::Rotation rotation;
};

struct Position {
    int x, y;
};
