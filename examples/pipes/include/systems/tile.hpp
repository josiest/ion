#pragma once

#include "entities/tile.hpp"

namespace tiles {

inline void increment_rotation(Rotation & rotation)
{
    switch (rotation) {
    case Rotation::Right: rotation = Rotation::Up; break;
    case Rotation::Up: rotation = Rotation::Left; break;
    case Rotation::Left: rotation = Rotation::Down; break;
    case Rotation::Down: rotation = Rotation::Right; break;
    default: break;
    }
}

inline void decrement_rotation(Rotation & rotation)
{
    switch (rotation) {
    case Rotation::Right: rotation = Rotation::Down; break;
    case Rotation::Up: rotation = Rotation::Right; break;
    case Rotation::Left: rotation = Rotation::Up; break;
    case Rotation::Down: rotation = Rotation::Left; break;
    default: break;
    }
}

template<typename engine_t>
Name random_name(engine_t & rng)
{
    std::uniform_int_distribution<> index(0, names.size()-1);
    auto choice = names.begin();
    std::advance(choice, index(rng));
    return *choice;
}

template<typename engine_t>
Rotation random_rotation(engine_t & rng)
{
    std::uniform_int_distribution<> index(0, rotations.size()-1);
    auto choice = rotations.begin();
    std::advance(choice, index(rng));
    return *choice;
}

void move(entt::registry & registry, entt::entity tile, int x, int y);
void rotate(entt::registry & registry, entt::entity tile, Rotation rotation);
bool is_adjacent(pointset const & tileset, int x, int y);

}
