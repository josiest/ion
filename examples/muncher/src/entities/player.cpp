#include "entities/player.hpp"
#include "components.hpp"

#include <SDL2/SDL_video.h>
#include <entt/entity/registry.hpp>

#include <cstdint>

namespace prefab {

player::player(SDL_Window * window) noexcept

      // player starts in the middle of the screen
    : _window{window},

      // 15 seems like a decent starting size
      _size{15.f},

      // player's color is black
      _r{0}, _g{0}, _b{0},

      // some arbitrary constants that seem to work decently for now
      _acceleration{200.f}, _friction{100.f},
      _minspeed{.1f}, _maxspeed{160.f}, _growth_rate{.06f}
{}

entt::entity player::create(entt::registry & registry) const noexcept
{
    auto player = registry.create();

    int width, height;
    SDL_GetWindowSize(_window, &width, &height);

    registry.emplace<component::bbox>(player, width/2.f, height/2.f, _size);
    registry.emplace<component::velocity>(player, 0.f, 0.f);
    registry.emplace<component::color>(player, _r, _g, _b);
    registry.emplace<component::dynamic_mover>(player, _acceleration, _friction,
                                                       _minspeed, _maxspeed);
    registry.emplace<component::growth_rate>(player, _growth_rate);
    return player;
}

component::bbox
player::try_get_bbox(entt::registry const & entities,
                     entt::entity playerid) const noexcept
{
    // return the player's position if they exist
    if (entities.valid(playerid) and
            entities.all_of<component::bbox>(playerid)) {

        return entities.get<component::bbox>(playerid);
    }
    // otherwise return their default starting size
    int width, height;
    SDL_GetWindowSize(_window, &width, &height);
    return {width/2.f, height/2.f, _size};
}
}
