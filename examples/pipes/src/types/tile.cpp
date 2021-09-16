#include "types/tile.hpp"
#include <iostream>

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
}
