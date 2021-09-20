#include "entities/player.hpp"
#include "components.hpp"

#include <SDL2/SDL.h>
#include <entt/entity/registry.hpp>

#include <cstdint>

namespace prefab {

player::player(std::uint32_t width, std::uint32_t height) noexcept

      // player starts in the middle of the screen
    : _x{width/2.f}, _y{height/2.f},

      // 15 seems like a decent starting size
      _size{15.f},

      // player's color is black
      _r{0}, _g{0}, _b{0},

      // some arbitrary constants that seem to work decently for now
      _acceleration{200.f}, _friction{100.f},
      _minspeed{0.1f}, _maxspeed{160.f}
{}

entt::entity player::create(entt::registry & registry) const noexcept
{
    auto player = registry.create();
    registry.emplace<component::position>(player, _x, _y);
    registry.emplace<component::velocity>(player, 0.f, 0.f);
    registry.emplace<component::size>(player, _size);
    registry.emplace<component::color>(player, _r, _g, _b);
    registry.emplace<component::dynamic_mover>(player, _acceleration, _friction,
                                                       _minspeed, _maxspeed);
    return player;
}

component::position
player::try_get_position(entt::registry const & entities,
                         entt::entity playerid) const noexcept
{
    // return the player's position if they exist
    if (entities.valid(playerid) and
            entities.all_of<component::position>(playerid)) {

        return entities.get<component::position>(playerid);
    }
    // otherwise return their default starting size
    return {_x, _y};
}

float player::try_get_size(entt::registry const & entities,
                           entt::entity playerid) const noexcept
{
    // return the player's size if they exist
    if (entities.valid(playerid) and
            entities.all_of<component::size>(playerid)) {

        return entities.get<component::size>(playerid).value;
    }
    // otherwise return their default starting size
    return _size;
}
}
