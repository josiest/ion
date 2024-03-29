#pragma once

#include <SDL.h>
#include <unordered_set>
#include <iostream>

namespace component {

namespace tileinfo {

// the possible tilenames
enum class name { end, bar, bend, fork };
inline std::unordered_set<name> names = {
    name::end, name::bar, name::bend, name::fork
};

// the possible tile rotations
enum class rotation { right, up, left, down };
inline std::unordered_set<rotation> rotations = {
    rotation::right, rotation::up, rotation::left, rotation::down
};

/** Print a tile name */
std::ostream & operator<<(std::ostream & os, name tilename);
/** Print a tile rotation */
std::ostream & operator<<(std::ostream & os, rotation rot);
}

struct tile {
    tileinfo::name name;
    tileinfo::rotation rotation;
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

namespace std {
template<> struct hash<component::tile> {
    size_t operator()(component::tile const & tile) const noexcept
    {
        hash<component::tileinfo::name> hash_name;
        hash<component::tileinfo::rotation> hash_rotation;
        return hash_name(tile.name) ^ hash_rotation(tile.rotation);
    }
};
}
