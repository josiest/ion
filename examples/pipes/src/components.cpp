#include "components.hpp"
#include <iostream>

namespace component::tileinfo {
std::ostream & operator<<(std::ostream & os, name tilename)
{
    switch (tilename) {
    case name::end: os << "end"; break;
    case name::bar: os << "bar"; break;
    case name::bend: os << "bend"; break;
    case name::fork: os << "fork"; break;
    defaut: break;
    }
    return os;
}

std::ostream & operator<<(std::ostream & os, rotation rot)
{
    using namespace component::tileinfo;
    switch (rot) {
    case rotation::right: os << "right"; break;
    case rotation::up: os << "up"; break;
    case rotation::left: os << "left"; break;
    case rotation::down: os << "down"; break;
    default: break;
    }
    return os;
}
}
