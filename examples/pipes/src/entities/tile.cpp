#include "entities/tile.hpp"
#include "tiles.hpp"
#include "components.hpp"
#include <entt.hpp>

entt::entity make_tile(entt::registry & registry,
                       tiles::TileName name, tiles::Rotation rot,
                       int x, int y)
{
    auto const tile = registry.create();
    registry.emplace<Tile>(tile, name, rot);
    registry.emplace<Position>(tile, x, y);
    return tile;
}

void move_tile(entt::registry & registry,
               entt::entity tile, int x, int y)
{
    auto & q = registry.get<Position>(tile);
    q.x = x;
    q.y = y;
}

void rotate_tile(entt::registry & registry,
                 entt::entity tile, tiles::Rotation rot)
{
    registry.get<Tile>(tile).rot = rot;
}
