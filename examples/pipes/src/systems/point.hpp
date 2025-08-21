#pragma once

#include "components.hpp"
#include <entt/entt.hpp>

#include <unordered_map>
#include <algorithm>
#include <concepts>

/** A polymorphic point concept */
template<class point_t>
concept point = requires(point_t p) {
    { p.x } -> std::convertible_to<int>;
    { p.y } -> std::convertible_to<int>;
};

/** Define how to hash a point-like object */
template<point point_t>
struct std::hash<point_t>
{
    size_t operator()(point_t const & p) const noexcept
    {
        constexpr hash<int> hash_int;
        return hash_int(p.x) ^ hash_int(p.y);
    }
};

/** Define how to compare two point-like objects for equality */
constexpr bool operator==(point auto const & p,
                          point auto const & q)
{
    return p.x == q.x && p.y == q.y;
}

/** An alias for a set of points */
using pointset = std::unordered_map<SDL_Point, entt::entity>;

namespace systems {

/** Determine if a tile is adjacent to others */
inline bool is_adjacent(pointset const & placed_tiles, int x, int y)
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

/** Determine if a tile is adjacent to others */
template<point point_t>
bool is_adjacent(pointset const & placed_tiles, point_t const & p)
{
    return is_adjacent(placed_tiles, p.x, p.y);
}
}
