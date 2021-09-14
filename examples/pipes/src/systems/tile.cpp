#include "systems/tile.hpp"
#include "entities/tile.hpp"
#include "components.hpp"

#include <entt.hpp>

namespace tiles {

void move(entt::registry & registry, entt::entity tile, int x, int y)
{
    auto & p = registry.get<Position>(tile);
    p.x = x;
    p.y = y;
}

void rotate(entt::registry & registry,
            entt::entity tile, tiles::Rotation rotation)
{
    registry.get<Tile>(tile).rotation = rotation;
}

}
