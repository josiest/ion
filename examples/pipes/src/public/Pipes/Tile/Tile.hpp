#pragma once
#include "Pipes/Tile/TileInfo.hpp"
#include <SDL2/SDL_pixels.h>

namespace Pipes::Component
{
struct Tile
{
    TileInfo::Name name;
    TileInfo::Rotation rotation;
    SDL_Color color;
};
}
