#pragma once

#include "components.hpp"

#include <entt/entity/registry.hpp>
#include <ion/input.hpp>

namespace systems {

/**
 * Accelerate the player in the direction of the input
 *
 * \param entities the registry the player may exist in
 * \param player the id of the player component
 * \param input the input axis used that interfaces with the player
 * \param dt the time since the last frame in seconds
 *
 * If the player exists in the entities, accelerate them in the direction of the
 * input with respect to the time since the last frame.
 */
void accelerate_player(entt::registry & entities, entt::entity player,
                       ion::input::axis2d const & input, float dt);

/**
 * Move all game entities according to their velocity
 *
 * \param entities the registry to move entities in
 * \param dt the time since the last frame in seconds
 *
 * All entities with position and velocity compoents will be moved according to
 * their velocity with respect to the time since the last frame.
 */
void move_munchies(entt::registry & entities, float dt);

/**
 * Determine if two bounding boxes are colliding
 *
 * \param a one of the bounding boxes to check against
 * \param b the other bounding box to check against
 */
bool collides_with(component::bbox const & a, component::bbox const & b);

/**
 * Compute the normalized velocity
 * \param v the velocity to normalize
 */
component::velocity normalized(component::velocity const & v, float eps);

/**
 * Compute the magnitude of a velocity
 * \param v the velocity to take the magnitude of
 */
float magnitude(component::velocity const & v);

component::velocity operator+(component::velocity const & lhs,
                              component::velocity const & rhs);

component::velocity & operator+=(component::velocity & lhs,
                                 component::velocity const & rhs);

component::velocity operator*(float lhs, component::velocity const & rhs);
}
