#pragma once
#include <SDL2/SDL.h>
#include "tiles.hpp"

struct Tile {
    tiles::TileName name;
    tiles::Rotation rot;
};

struct Position {
    int x, y;
};
