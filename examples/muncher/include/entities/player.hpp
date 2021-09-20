#pragma once
#include "components.hpp"
#include <entt/entity/registry.hpp>

#include <cstdint>

namespace prefab {

class player {
public:
    // delete unwanted implicit constructors
    player() = delete;
    player(player const &) = delete;

    /**
     * Create a player prefab within the given bounds
     *
     * \param width the width of the bounds
     * \param height the height of the bounds
     */
    player(std::uint32_t width, std::uint32_t height) noexcept;

    /**
     * Create a player entity in a registry
     *
     * \param entities the registry to create the player in
     * \return the id of the newly created player entity
     */
    entt::entity create(entt::registry & entities) const noexcept;

    /**
     * Get the player's bbox component or default to their starting bbox
     *
     * \param entities the registry where the player may exist
     * \param playerid the id of the player entity
     *
     * \return the player's bbox if they exist in entities. Otherwise return
     *         their default starting bbox.
     */
    component::bbox try_get_bbox(entt::registry const & entities,
                                 entt::entity playerid) const noexcept;
private:
    float _x, _y;
    float _size;
    std::uint8_t _r, _g, _b;
    float _acceleration, _friction;
    float _minspeed, _maxspeed;
};
}
