#include "entities/tile.hpp"
#include "components.hpp"

#include <ion/graphics.hpp>
#include <SDL2/SDL_surface.h>
#include <entt.hpp>

#include <ostream>
#include <sstream>

#include <unordered_map>

namespace tiles {

std::ostream & operator<<(std::ostream & os, Name tilename)
{
    switch (tilename) {
    case Name::End: os << "end"; break;
    case Name::Bar: os << "bar"; break;
    case Name::Bend: os << "bend"; break;
    case Name::Fork: os << "fork"; break;
    defaut: break;
    }
    return os;
}

std::ostream & operator<<(std::ostream & os, Rotation rotation)
{
    switch (rotation) {
    case Rotation::Right: os << "right"; break;
    case Rotation::Up: os << "up"; break;
    case Rotation::Left: os << "left"; break;
    case Rotation::Down: os << "down"; break;
    default: break;
    }
    return os;
}

std::string image_path(Name tilename, Rotation rotation)
{
    std::stringstream path;
    path << "../resources/" << tilename << "-" << rotation << ".bmp";
    return path.str();
}

map::map()
{
    for (Name tilename : names) {
        std::unordered_map<Rotation, ion::surface> surfaces;
        for (Rotation rotation : rotations) {
            auto const path = image_path(tilename, rotation);
            surfaces.try_emplace(rotation, ion::load_bitmap_image(path));
        }
        _images.emplace(tilename, surfaces);
    }
}

SDL_Surface * map::sdl_surface(Name tilename, Rotation rotation)
{
    return _images.at(tilename).at(rotation).sdl_surface();
}

SDL_Surface const * map::sdl_surface(Name tilename, Rotation rotation) const
{
    return _images.at(tilename).at(rotation).sdl_surface();
}

entt::entity make(entt::registry & registry,
                  Name name, Rotation rotation, int x, int y)
{
    auto const tile = registry.create();
    registry.emplace<Tile>(tile, name, rotation);
    registry.emplace<Position>(tile, x, y);
    return tile;
}

void move(entt::registry & registry, entt::entity tile, int x, int y)
{
    auto & p = registry.get<Position>(tile);
    p.x = x;
    p.y = y;
}

void rotate(entt::registry & registry,
            entt::entity tile, tiles::Rotation rotation)
{
    registry.get<Tile>(tile).rotation = rotation;
}
}
