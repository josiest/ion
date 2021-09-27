#pragma once

#include <entt/entity/registry.hpp>
#include <SDL.h>

#include <unordered_map>
#include <concepts>

/** A polymporhic point concept */
template<class point_t>
concept point = requires(point_t p) {
    { p.x } -> std::convertible_to<int>;
    { p.y } -> std::convertible_to<int>;
};

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
using pointset = std::unordered_map<SDL_Point, entt::entity>;
