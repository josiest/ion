#include "systems/tile.hpp"
#include "entities/tile.hpp"
#include "components.hpp"

#include <entt.hpp>

namespace tiles {

entt::entity make(entt::registry & registry,
                  Name name, Rotation rotation, int x, int y)
{
    auto const tile = registry.create();
    registry.emplace<Tile>(tile, name, rotation);
    registry.emplace<Position>(tile, x, y);
    return tile;
}

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
