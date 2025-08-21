#pragma once
#include "Pipes/Tile/TileInfo.hpp"
#include "Pipes/Tile/Tile.hpp"
#include <entt/entt.hpp>


namespace systems
{

/** Randomize the tilename and rotation of a tile */
template<class engine_t>
void randomize_tile(entt::registry & entities, entt::entity entity, engine_t & rng)
{
    auto & tile = entities.get<Pipes::Component::Tile>(entity);
    tile.name = Pipes::TileInfo::random_name(rng);
    tile.rotation = Pipes::TileInfo::random_rotation(rng);
}
}
