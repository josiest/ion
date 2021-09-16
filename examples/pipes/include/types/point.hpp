#pragma once

#include <concepts>

#include <unordered_set>
#include <SDL2/SDL_rect.h>

/**
 * A polymorphic int-point concept
 */
template<typename point_t>
concept point =
    std::same_as<decltype(point_t::x), int> &&
    std::same_as<decltype(point_t::y), int>;

/**
 * Define how to hash a point-like object
 */
namespace std {
template<point point_t> struct hash<point_t> {
    size_t operator()(point_t const & p) const noexcept
    {
        hash<int> hashint;
        return hashint(p.x) ^ hashint(p.y);
    }
};
}

/**
 * Define how to compare two point-like objects for equality
 */
constexpr bool operator==(point auto const & p, point auto const & q) noexcept
{
    return p.x == q.x && p.y == q.y;
}

/**
 * An alias for a set of SDL_Points
 */
using pointset = std::unordered_set<SDL_Point>;
