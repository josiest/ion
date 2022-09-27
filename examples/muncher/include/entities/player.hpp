#pragma once
#include "components.hpp"
#include <entt/entity/registry.hpp>
#include <SDL2/SDL_video.h>

class player_spawner {
public:
    // delete unwanted implicit constructors
    player_spawner() = delete;
    player_spawner(player_spawner const &) = delete;

    // resource management
    player_spawner(player_spawner && temp) = default;

    static tl::expected<player_spawner, std::string>
    from_config(std::string const & config_path,
                SDL_Renderer * renderer);

    /**
     * Create a player entity in a registry
     *
     * \param entities the registry to create the player in
     * \return the id of the newly created player entity
     */
    entt::entity make_player(entt::registry & entities) const;

    /**
     * Get the player's bbox component or default to their starting bbox
     *
     * \param entities the registry where the player may exist
     * \param playerid the id of the player entity
     *
     * \return the player's bbox if they exist in entities. Otherwise return
     *         their default starting bbox.
     */
    component::bbox bbox(entt::registry const & entities,
                         entt::entity playerid) const;
private:
    /** Reference to the bounds to spawn the player in */
    SDL_Renderer * _renderer;

    /** What color should the player be? */
    component::color _color;

    /** How big, in pixels should the player be? */
    float _size;

    /** How quickly should the player accelerate and deaccelerate? */
    float _acceleration, _friction;

    /** How fast can the player move, and how slow before they stop? */
    float _minspeed, _maxspeed;

    /** How much of the munchable's size should the player grow by? */
    float _growth_rate;

    player_spawner(SDL_Renderer * renderer, component::color const & color,
                   float size, float acceleration, float friction,
                   float minspeed, float maxspeed, float growth_rate);
};
}
