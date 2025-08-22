#pragma once

#include "Pipes/PointSet.hpp"
#include <entt/entt.hpp>

#include <unordered_map>
#include <algorithm>
#include <concepts>

namespace systems {

/** Determine if a tile is adjacent to others */
inline bool is_adjacent(Pipes::PointSet const & placed_tiles, int x, int y)
{
    // a list of adjacent points
    std::vector<SDL_Point> points{
        {x+1, y}, {x-1, y}, {x, y+1}, {x, y-1}
    };
    // determine if an adjacent tile has been placed
    return std::ranges::any_of(points, [&placed_tiles](auto const & p) {
        return placed_tiles.contains(p);
    });
}

/** A polymorphic point concept */
template<class point_t>
concept point = requires(point_t p) {
    { p.x } -> std::convertible_to<int>;
    { p.y } -> std::convertible_to<int>;
};

/** Determine if a tile is adjacent to others */
template<point point_t>
bool is_adjacent(Pipes::PointSet const & placed_tiles, point_t const & p)
{
    return is_adjacent(placed_tiles, p.x, p.y);
}
}
