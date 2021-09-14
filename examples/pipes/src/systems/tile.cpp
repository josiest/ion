#include "systems/tile.hpp"
#include "entities/tile.hpp"
#include "components.hpp"

#include <entt.hpp>

#include <vector>
#include <numeric>

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

bool is_adjacent(pointset const & tileset, int x, int y)
{
    std::vector<SDL_Point> points{
        {x+1, y}, {x-1, y}, {x, y+1}, {x, y-1}
    };
    auto fold_or = [&tileset](bool adj, SDL_Point const & p) {
        return adj || tileset.contains(p);
    };
    return std::accumulate(points.begin(), points.end(), false, fold_or);
}
}
