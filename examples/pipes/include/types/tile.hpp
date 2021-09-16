#pragma once

#include <unordered_set>
#include <utility>

#include <unordered_map>
#include <ion/graphics/image.hpp>

namespace tiles {

enum class Name { End, Bar, Bend, Fork };
inline std::unordered_set<Name> names = {
    Name::End, Name::Bar, Name::Bend, Name::Fork
};
std::ostream & operator<<(std::ostream & os, Name tilename);

enum class Rotation { Right, Up, Left, Down };
inline std::unordered_set<Rotation> rotations = {
    Rotation::Right, Rotation::Up, Rotation::Left, Rotation::Down
};
std::ostream & operator<<(std::ostream & os, Rotation rot);

using pair = std::pair<Name, Rotation>;
inline bool operator==(pair const & a, pair const & b)
{
    return a.first == b.first and a.second == b.second;
}
}

namespace std {
template<> struct hash<tiles::pair> {
    size_t operator()(tiles::pair const & tile) const noexcept
    {
        hash<tiles::Name> hash_name;
        hash<tiles::Rotation> hash_rotation;
        return hash_name(tile.first) ^ hash_rotation(tile.second);
    }
};
}

using tilemap = std::unordered_map<tiles::pair, ion::unique_surface>;
