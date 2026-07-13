#include "entities/player.hpp"
#include "components.hpp"

#include <entt/entity/registry.hpp>
#include <SDL3/SDL_video.h>

namespace prefab {

player::player()

    : // 15 seems like a decent starting size
      _size{15.f},

      // player's color is black
      _r{0}, _g{0}, _b{0},

      // some arbitrary constants that seem to work decently for now
      _acceleration{200.f}, _friction{100.f},
      _min_speed{.1f}, _max_speed{160.f}, _growth_rate{.06f}
{}

entt::entity player::create(entt::registry & registry, SDL_FRect bounds) const
{
    auto player = registry.create();

    registry.emplace<component::bbox>(player, bounds.w/2.f, bounds.h/2.f, _size);
    registry.emplace<component::velocity>(player, 0.f, 0.f);
    registry.emplace<component::color>(player, _r, _g, _b);
    registry.emplace<component::dynamic_mover>(player, _acceleration, _friction,
                                                       _min_speed, _max_speed);
    registry.emplace<component::growth_rate>(player, _growth_rate);
    return player;
}

component::bbox
player::try_get_bbox(entt::registry const & entities, entt::entity player_id, SDL_FRect bounds) const
{
    // return the player's position if they exist
    if (entities.valid(player_id) and
            entities.all_of<component::bbox>(player_id)) {

        return entities.get<component::bbox>(player_id);
    }
    // otherwise return their default starting size
    return {bounds.w/2.f, bounds.h/2.f, _size};
}
}
