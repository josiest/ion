#pragma once

#include <entt/entity/registry.hpp>
#include <cstdint>

namespace systems {

/**
 * The player munches or gets munched.
 *
 * \param entities the registry to interact with
 * \param player the id of the player entity
 *
 * If the player exists in the registry have them munch any munchables they're
 * currently colliding with that have a smaller size than them. If the munchable
 * is at least the same size, they munch the player!
 */
void munch(entt::registry & entities, entt::entity player);

/**
 * Filter out all munchables that are out of bounds
 *
 * \param entities the registry to filter munchables
 * \param width the width of the bounds
 * \param height the height of the bounds
 */
void filter_munchables(entt::registry & entities,
                       std::uint32_t width, std::uint32_t height);
}
