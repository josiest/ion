#include "components.hpp"
#include "systems/tile.hpp"

namespace systems {

using namespace component;
void increment_rotation(tileinfo::rotation & rot)
{
    using namespace tileinfo;
    switch (rot) {
    case rotation::right: rot = rotation::up; break;
    case rotation::up: rot = rotation::left; break;
    case rotation::left: rot = rotation::down; break;
    case rotation::down: rot = rotation::right; break;
    default: break;
    }
}

void decrement_rotation(tileinfo::rotation & rot)
{
    using namespace tileinfo;
    switch (rot) {
    case rotation::right: rot = rotation::down; break;
    case rotation::up: rot = rotation::right; break;
    case rotation::left: rot = rotation::up; break;
    case rotation::down: rot = rotation::left; break;
    default: break;
    }
}
}
