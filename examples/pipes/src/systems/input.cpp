#include "systems/input.hpp"
#include "entities/tile.hpp"

#include "grid.hpp"
#include "components.hpp"

#include <entt.hpp>
#include <SDL2/SDL.h>

void bind_to_mouse(entt::registry & registry, Grid const & grid,
                   entt::entity entity)
{
    // get the mouse coordinates and translate them to grid-space
    SDL_Point mouse{0, 0};
    SDL_GetMouseState(&mouse.x, &mouse.y);
    auto q = grid.nearest_point(mouse);

    // update the desired entity to correspond to the mouse position
    move_tile(registry, entity, q.x, q.y);
}
