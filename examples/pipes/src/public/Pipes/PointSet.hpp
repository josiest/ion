#pragma once
#include <SDL2/SDL_rect.h>
#include <entt/entity/entity.hpp>
#include <unordered_map>

template<>
struct std::hash<SDL_Point>
{
    size_t operator()(const SDL_Point & p) const noexcept
    {
        constexpr hash<int> hash_int;
        return hash_int(p.x) ^ hash_int(p.y);
    }
};

constexpr bool operator==(const SDL_Point & lhs, const SDL_Point & rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

namespace Pipes
{
/** An alias for a set of points */
using PointSet = std::unordered_map<SDL_Point, entt::entity>;
}