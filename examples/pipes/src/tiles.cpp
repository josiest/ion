#include "tiles.hpp"
#include <ion/render.hpp>
#include <SDL2/SDL_surface.h>

#include <ostream>
#include <sstream>

#include <unordered_map>

namespace tiles {

std::ostream & operator<<(std::ostream & os, TileName tile)
{
    switch (tile) {
    case TileName::End: os << "end"; break;
    case TileName::Bar: os << "bar"; break;
    case TileName::Bend: os << "bend"; break;
    case TileName::Fork: os << "fork"; break;
    defaut: break;
    }
    return os;
}

std::ostream & operator<<(std::ostream & os, Rotation rot)
{
    switch (rot) {
    case Rotation::Right: os << "right"; break;
    case Rotation::Up: os << "up"; break;
    case Rotation::Left: os << "left"; break;
    case Rotation::Down: os << "down"; break;
    default: break;
    }
    return os;
}

std::string image_path(TileName tile, Rotation rot)
{
    std::stringstream name;
    name << "../resources/" << tile << "-" << rot << ".bmp";
    return name.str();
}

TileMap::TileMap()
{
    for (TileName tile : names) {
        std::unordered_map<Rotation, ion::Bitmap> rots;
        for (Rotation rot : rotations) {
            auto const path = image_path(tile, rot);
            rots.try_emplace(rot, ion::load_bitmap_image(path));
        }
        _images.emplace(tile, rots);
    }
}

SDL_Surface * TileMap::sdl_surface(TileName tile, Rotation rot)
{
    return _images.at(tile).at(rot).sdl_surface();
}

SDL_Surface const * TileMap::sdl_surface(TileName tile, Rotation rot) const
{
    return _images.at(tile).at(rot).sdl_surface();
}
}
