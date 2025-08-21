#include "Pipes/Tile/TileInfo.hpp"
#include "components.hpp"
#include "systems/grid.hpp"

#include <entt/entt.hpp>
#include <SDL2/SDL.h>

#include "Pipes/Tile/Tile.hpp"

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
    auto & tile = entities.get<Pipes::Component::Tile>(mouse_tile);
    if (dy > 0) { ++tile.rotation; }
    else if (dy < 0) { --tile.rotation; }
}

}
