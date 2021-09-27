#include "systems/tile.hpp"
#include "entities/tile.hpp"
#include "types/components.hpp"

#include <entt/entity/registry.hpp>

#include <vector>
#include <numeric>

namespace systems {

void move_tile(entt::registry & registry, entt::entity tile, int x, int y)
{
    auto & p = registry.get<component::position>(tile);
    p.x = x;
    p.y = y;
}

void rotate_tile(entt::registry & registry,
                 entt::entity tile, tiles::Rotation rotation)
{
    registry.get<component::tile>(tile).rotation = rotation;
}

bool is_adjacent(pointset const & tileset, int x, int y)
{
    std::vector<SDL_Point> points{
        {x+1, y}, {x-1, y}, {x, y+1}, {x, y-1}
    };
    auto fold_or = [&tileset](bool is_adjacent, SDL_Point const & p) {
        return is_adjacent or tileset.contains(p);
    };
    return std::accumulate(points.begin(), points.end(), false, fold_or);
}
}
