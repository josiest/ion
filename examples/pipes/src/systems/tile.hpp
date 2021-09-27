#pragma once

#include "types/components.hpp"
#include "types/tile.hpp"
#include "systems/point.hpp"

#include <entt/entity/registry.hpp>

#include <random>
#include <iterator>

namespace systems {

/** Increment a rotation in the counter-clockwise direction */
inline void increment_rotation(tiles::Rotation & rotation)
{
    using namespace tiles;
    switch (rotation) {
    case Rotation::Right: rotation = Rotation::Up; break;
    case Rotation::Up: rotation = Rotation::Left; break;
    case Rotation::Left: rotation = Rotation::Down; break;
    case Rotation::Down: rotation = Rotation::Right; break;
    default: break;
    }
}

/** Decrement a rotation in the clockwise direction */
inline void decrement_rotation(tiles::Rotation & rotation)
{
    using namespace tiles;
    switch (rotation) {
    case Rotation::Right: rotation = Rotation::Down; break;
    case Rotation::Up: rotation = Rotation::Right; break;
    case Rotation::Left: rotation = Rotation::Up; break;
    case Rotation::Down: rotation = Rotation::Left; break;
    default: break;
    }
}

/** Generate a random tilename */
template<typename engine_t>
tiles::Name random_name(engine_t & rng)
{
    std::uniform_int_distribution<> index(0, tiles::names.size()-1);
    auto choice = tiles::names.begin();
    std::advance(choice, index(rng));
    return *choice;
}

/** Generate a random rotation */
template<typename engine_t>
tiles::Rotation random_rotation(engine_t & rng)
{
    std::uniform_int_distribution<> index(0, tiles::rotations.size()-1);
    auto choice = tiles::rotations.begin();
    std::advance(choice, index(rng));
    return *choice;
}

/** Randomize the tilename and rotation of a tile */
template<class engine_t>
void randomize_tile(entt::registry & entities, entt::entity entity,
                    engine_t & rng)
{
    auto & tile = entities.get<component::tile>(entity);
    tile.name = random_name(rng);
    tile.rotation = random_rotation(rng);
}

}
