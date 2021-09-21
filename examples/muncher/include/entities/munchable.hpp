#pragma once
#include "entities/player.hpp"
#include "components.hpp"

#include <entt/entity/registry.hpp>
#include <SDL2/SDL_video.h>

#include <random>
#include <vector>

namespace prefab {

using engine_t = std::mt19937;

/**
 * A prefab munchable that with random distribution parameters
 */
class munchable {
public:

    // delete any unwanted implicit constructors
    munchable() = delete;
    munchable(munchable const &) = delete;
    /**
     * Create a munchable prefab with bounds connected to a window's size
     * \param window the window that defines the bounds
     */
    explicit munchable(SDL_Window * window) noexcept;

    /**
     * Create a munchable entity in a registry
     *
     * \param entities the registry to create the munchable in
     * \param playerid the id of the player entity in the registry
     * \param player_settings the default player settings
     * \param rng the random number generator
     *
     * \return the id of the newly created munchable entity
     */
    entt::entity
    create(entt::registry & entities, entt::entity playerid,
           player const & player_settings, engine_t rng) const noexcept;

    /**
     * Determine if a munchable entity should be created
     *
     * \param dt the time since the last frame in seconds
     * \param rng the random number generator
     */
    bool should_munch(float dt, engine_t & rng) const noexcept;
private:
    SDL_Window * _window;
    std::vector<component::color> _colors;

    float _minspeed, _maxspeed, _angle_variation;
    float _size_variation;
    float _munchtime_likelihood;

    /**
     * Generate a random bbox component for a munchable
     *
     * \param entities the registry where the munchable exists
     * \param munchableid the id of the munchable entity
     * \param player_box the bounding box of the player
     * \param rng the random number generator
     *
     * Behavior is undefined if munchableid doesn't exist in entities.
     */
    component::bbox &
    random_bbox(entt::registry & entities, entt::entity munchableid,
                component::bbox const & player_box,
                engine_t & rng) const noexcept;

    /**
     * Generate a random velocity component for a munchable
     *
     * \param entities the registry where the munchable exists
     * \param munchableid the id of the munchable entity
     *
     * \param munchable_box the bounding box of the munchable
     * \param player_box the bounding box of the player
     *
     * \param rng the random number generator
     *
     * Behavior is undefined if munchableid doesn't exist in entities
     */
    component::velocity &
    random_velocity(entt::registry & entities, entt::entity munchableid,
                    component::bbox const & munchable_box,
                    component::bbox const & player_box,
                    engine_t & rng) const noexcept;

    /**
     * Generate a random color component for a munchable
     *
     * \param entities the registry where the munchable exists
     * \param munchableid the id of the munchable entity
     * \param rng the random number generator
     *
     * Behavior is undefined if munchableid doesn't exist in entities
     */
    component::color &
    random_color(entt::registry & entitites, entt::entity munchableid,
                 engine_t & rng) const noexcept;
};
}
