#pragma once
#include "components.hpp"
#include <entt/entity/registry.hpp>

struct SDL_Window;

namespace prefab {

class player {
public:
    player();

    /**
     * Create a player entity in a registry
     *
     * \param entities the registry to create the player in
     * \param bounds the bounds to spawn the player in
     * \return the id of the newly created player entity
     */
    entt::entity create(entt::registry & entities, SDL_FRect bounds) const;

    /**
     * Get the player's bbox component or default to their starting bbox
     *
     * \param entities the registry where the player may exist
     * \param player_id the id of the player entity
     * \param bounds the bounds to spawn the player in
     *
     * \return the player's bbox if they exist in entities. Otherwise, return
     *         their default starting bbox.
     */
    component::bbox try_get_bbox(entt::registry const & entities,
                                 entt::entity player_id, SDL_FRect bounds) const;
private:
    float _size;
    std::uint8_t _r, _g, _b;
    float _acceleration, _friction;
    float _min_speed, _max_speed;
    float _growth_rate;
};
}
