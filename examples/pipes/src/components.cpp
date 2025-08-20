#include "components.hpp"
#include <iostream>

std::ostream & operator<<(std::ostream & os, component::tileinfo::name tilename)
{
    using namespace component::tileinfo;;
    switch (tilename) {
    case name::end: os << "end"; break;
    case name::bar: os << "bar"; break;
    case name::bend: os << "bend"; break;
    case name::fork: os << "fork"; break;
    default: break;
    }
    return os;
}

std::ostream & operator<<(std::ostream & os, component::tileinfo::rotation rot)
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