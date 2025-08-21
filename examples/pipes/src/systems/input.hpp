#pragma once
#include "systems/grid.hpp"
#include <entt/entt.hpp>

namespace systems {

/** Bind a tile to the mouse position */
void bind_to_mouse(entt::registry & entities, entt::entity tile,
                   grid const & world_space);

/** Rotate a tile in the direction of a mouse scroll */
void rotate_tile(entt::registry & entities, entt::entity tile, int dy);
}
