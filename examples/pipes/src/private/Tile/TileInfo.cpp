#include "Pipes/Tile/TileInfo.hpp"

std::ostream & operator<<(std::ostream & os, Pipes::TileInfo::Name tilename)
{
    using namespace Pipes::TileInfo;;
    switch (tilename) {
        case Name::End: os << "end"; break;
        case Name::Bar: os << "bar"; break;
        case Name::Bend: os << "bend"; break;
        case Name::Fork: os << "fork"; break;
        default: break;
    }
    return os;
}

std::ostream & operator<<(std::ostream & os, Pipes::TileInfo::Rotation rot)
{
    using namespace Pipes::TileInfo;
    switch (rot) {
        case Rotation::Right: os << "right"; break;
        case Rotation::Up: os << "up"; break;
        case Rotation::Left: os << "left"; break;
        case Rotation::Down: os << "down"; break;
        default: break;
    }
    return os;
}
