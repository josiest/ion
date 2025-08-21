#pragma once

#include "Pipes/Tile/TileInfo.hpp"
#include <SDL2/SDL.h>
#include <unordered_set>
#include <iostream>

namespace component {

struct tile
{
    Pipes::TileInfo::Name name;
    Pipes::TileInfo::Rotation rotation;
};

/** Compare two tiles for equality */
inline bool operator==(tile const & a, tile const & b)
{
    return a.name == b.name and a.rotation == b.rotation;
}

struct position {
    int x, y;
    inline operator SDL_Point() const { return {x, y}; }
};

struct static_tile {};
}

template<> struct std::hash<component::tile>
{
    size_t operator()(component::tile const & tile) const noexcept
    {
        constexpr hash<Pipes::TileInfo::Name> hash_name;
        constexpr hash<Pipes::TileInfo::Rotation> hash_rotation;
        return hash_name(tile.name) ^ hash_rotation(tile.rotation);
    }
};
