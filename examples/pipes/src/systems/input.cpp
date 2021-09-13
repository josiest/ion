#include "entities/tile.hpp"

#include "systems/input.hpp"
#include "systems/grid.hpp"

#include "components.hpp"

#include <entt.hpp>
#include <SDL2/SDL.h>

void bind_to_mouse(grid const & world_space, entt::registry & entities,
                   entt::entity tile)
{
    // get the mouse coordinates and translate them to grid-space
    SDL_Point mouse{0, 0};
    SDL_GetMouseState(&mouse.x, &mouse.y);
    auto q = world_space.nearest_point(mouse);

    // update the desired entity to correspond to the mouse position
    tiles::move(entities, tile, q.x, q.y);
}
