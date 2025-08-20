#pragma once

#include "components.hpp"
#include "systems/point.hpp"

#include <entt/entt.hpp>

#include <random>
#include <iterator>

namespace systems
{
/** Increment a rotation in the counter-clockwise direction */
void increment_rotation(component::tileinfo::rotation & rotation);

/** Decrement a rotation in the clockwise direction */
void decrement_rotation(component::tileinfo::rotation & rotation);

/** Generate a random tilename */
template<typename engine_t>
component::tileinfo::name random_name(engine_t & rng)
{
    std::uniform_int_distribution<> index(0, component::tileinfo::names.size()-1);
    auto choice = component::tileinfo::names.begin();
    std::advance(choice, index(rng));
    return *choice;
}

/** Generate a random rotation */
template<typename engine_t>
component::tileinfo::rotation random_rotation(engine_t & rng)
{
    std::uniform_int_distribution<> index(0, component::tileinfo::rotations.size()-1);
    auto choice = component::tileinfo::rotations.begin();
    std::advance(choice, index(rng));
    return *choice;
}

/** Randomize the tilename and rotation of a tile */
template<class engine_t>
void randomize_tile(entt::registry & entities, entt::entity entity,
                    engine_t & rng)
{
    auto & [name, rotation] = entities.get<component::tile>(entity);
    name = random_name(rng);
    rotation = random_rotation(rng);
}
}
