#include "pipes.hpp"
#include "types/components.hpp"
#include "entities/tile.hpp"
#include "systems/tile.hpp"

#include <entt/entity/registry.hpp>
#include <SDL2/SDL.h>

namespace cmpt = component;

namespace systems {

void bind_to_mouse(entt::registry & entities, entt::entity tile,
                   grid const & world_space)
{
    // get the mouse coordinates and translate them to grid-space
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);

    // update the desired entity to correspond to the mouse position
    auto & p = entities.get<cmpt::position>(tile);
    p = world_space.nearest_point(mouse);
}

void rotate_tile(entt::registry & entities, entt::entity mouse_tile, int dy)
{
    // get a reference to the tile and change its rotation
    auto & tile = entities.get<cmpt::tile>(mouse_tile);
    if (dy > 0) { increment_rotation(tile.rotation); }
    else if (dy < 0) { decrement_rotation(tile.rotation); }
}

}
