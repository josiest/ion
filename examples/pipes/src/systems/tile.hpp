#pragma once
#include "Pipes/Tile/TileInfo.hpp"
#include "components.hpp"
#include <entt/entt.hpp>

namespace systems
{

/** Randomize the tilename and rotation of a tile */
template<class engine_t>
void randomize_tile(entt::registry & entities, entt::entity entity, engine_t & rng)
{
    auto & [name, rotation] = entities.get<component::tile>(entity);
    name = Pipes::TileInfo::random_name(rng);
    rotation = Pipes::TileInfo::random_rotation(rng);
}
}
