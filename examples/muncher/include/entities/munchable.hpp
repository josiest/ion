#pragma once

// game framework
// #include "entities/player.hpp"
#include "components.hpp"

// frameworks
#include <ion/graphics/renderer.hpp>
#include <entt/entity/registry.hpp>
#include <SDL2/SDL.h>

// data types
#include <string>
#include <vector>
#include <tl/expected.hpp>

// math
#include <random>

using engine_t = std::mt19937;

/**
 * Save configuration information for spawning munchables
 */
class munchable_spawner {
public:

    // delete any unwanted implicit constructors
    munchable_spawner() = delete;
    munchable_spawner(munchable_spawner const &) = delete;

    // resource management
    munchable_spawner(munchable_spawner && temp) = default;

    /**
     * \brief Load munchable_spawner settings from a config file.
     *
     * \param config_path   the path to the config file.
     */
    static tl::expected<munchable_spawner, std::string>
    from_config(std::string const & config_path,
                SDL_Renderer * renderer);
// 
//     /**
//      * Create a munchable entity in a registry
//      *
//      * \param entities the registry to create the munchable in
//      * \param playerid the id of the player entity in the registry
//      * \param player_settings the default player settings
//      * \param rng the random number generator
//      *
//      * \return the id of the newly created munchable entity
//      */
//     entt::entity
//     create(entt::registry & entities, entt::entity playerid,
//            player const & player_settings, engine_t rng) const;
// 
//     /**
//      * Determine if a munchable entity should be created
//      *
//      * \param dt the time since the last frame in seconds
//      * \param rng the random number generator
//      */
//     bool should_spawn(float dt, engine_t & rng) const;
private:
    /** Where should munchables be spawned? */
    SDL_Renderer * _renderer;

    /** How should munchables be colored? */
    std::vector<component::color> _colors;

    /** What velocities should munchables vary between? */
    float _minspeed, _maxspeed, _angle_variation;
    
    /** How should munchables vary in size? */
    float _size_variation;

    /** How likely is one to spawn at any given millisecond? */
    float _spawn_likelihood;

    munchable_spawner(
        SDL_Renderer * renderer,
        std::vector<component::color> const & colors,
        float minspeed, float maxspeed,
        float angle_variation,
        float size_variation,
        float spawn_likelihood);
// 
//     /**
//      * Generate a random bbox component for a munchable
//      *
//      * \param entities the registry where the munchable exists
//      * \param munchableid the id of the munchable entity
//      * \param player_box the bounding box of the player
//      * \param rng the random number generator
//      *
//      * Behavior is undefined if munchableid doesn't exist in entities.
//      */
//     component::bbox &
//     random_bbox(entt::registry & entities, entt::entity munchableid,
//                 component::bbox const & player_box,
//                 engine_t & rng) const;
// 
//     /**
//      * Generate a random velocity component for a munchable
//      *
//      * \param entities the registry where the munchable exists
//      * \param munchableid the id of the munchable entity
//      *
//      * \param munchable_box the bounding box of the munchable
//      * \param player_box the bounding box of the player
//      *
//      * \param rng the random number generator
//      *
//      * Behavior is undefined if munchableid doesn't exist in entities
//      */
//     component::velocity &
//     random_velocity(entt::registry & entities, entt::entity munchableid,
//                     component::bbox const & munchable_box,
//                     component::bbox const & player_box,
//                     engine_t & rng) const;
// 
//     /**
//      * Generate a random color component for a munchable
//      *
//      * \param entities the registry where the munchable exists
//      * \param munchableid the id of the munchable entity
//      * \param rng the random number generator
//      *
//      * Behavior is undefined if munchableid doesn't exist in entities
//      */
//     component::color &
//     random_color(entt::registry & entitites, entt::entity munchableid,
//                  engine_t & rng) const;
};
