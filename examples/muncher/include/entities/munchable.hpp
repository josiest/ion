#pragma once
#include "entities/player.hpp"
#include "components.hpp"

#include <entt/entity/registry.hpp>

#include <random>
#include <vector>

namespace prefab {

using engine_t = std::mt19937;

/**
 * A prefab munchable that with random distribution parameters
 */
class munchable {
public:
    explicit munchable();

    /**
     * Create a munchable entity in a registry
     *
     * \param entities the registry to create the munchable in
     * \param player_id the id of the player entity in the registry
     * \param bounds the bounds to spawn the munchable in
     * \param player_settings the default player settings
     * \param rng the random number generator
     *
     * \return the id of the newly created munchable entity
     */
    entt::entity
    create(entt::registry & entities, entt::entity player_id, SDL_FRect bounds,
           player const & player_settings, engine_t rng) const;

    /**
     * Determine if a munchable entity should be created
     *
     * \param dt the time since the last frame in seconds
     * \param rng the random number generator
     */
    bool should_munch(float dt, engine_t & rng) const;
private:
    std::vector<component::color> _colors;

    float _min_speed, _max_speed, _angle_variation;
    float _size_variation;
    float _munch_time_likelihood;

    /**
     * Generate a random bbox component for a munchable
     *
     * \param entities the registry where the munchable exists
     * \param munchable_id the id of the munchable entity
     * \param player_box the bounding box of the player
     * \param bounds the bounds to spawn the munchable in
     * \param rng the random number generator
     *
     * Behavior is undefined if munchable_id doesn't exist in entities.
     */
    component::bbox &
    random_bbox(entt::registry & entities, entt::entity munchable_id,
                component::bbox const & player_box, SDL_FRect bounds,
                engine_t & rng) const;

    /**
     * Generate a random velocity component for a munchable
     *
     * \param entities the registry where the munchable exists
     * \param munchable_id the id of the munchable entity
     *
     * \param munchable_box the bounding box of the munchable
     * \param player_box the bounding box of the player
     *
     * \param rng the random number generator
     *
     * Behavior is undefined if munchable_id doesn't exist in entities
     */
    component::velocity &
    random_velocity(entt::registry & entities, entt::entity munchable_id,
                    component::bbox const & munchable_box,
                    component::bbox const & player_box,
                    engine_t & rng) const;

    /**
     * Generate a random color component for a munchable
     *
     * \param entities the registry where the munchable exists
     * \param munchable_id the id of the munchable entity
     * \param rng the random number generator
     *
     * Behavior is undefined if munchable_id doesn't exist in entities
     */
    component::color &
    random_color(entt::registry & entities, entt::entity munchable_id,
                 engine_t & rng) const;
};
}
