#include "pipes.hpp"
#include "components.hpp"
#include "systems/tile.hpp"

#include <entt.hpp>
#include <SDL2/SDL.h>

void pipes::bind_to_mouse(entt::registry & entities, entt::entity tile)
{
    // get the mouse coordinates and translate them to grid-space
    SDL_Point mouse{0, 0};
    SDL_GetMouseState(&mouse.x, &mouse.y);
    auto q = _world_space.nearest_point(mouse);

    // update the desired entity to correspond to the mouse position
    tiles::move(entities, tile, q.x, q.y);
}

void pipes::rotate_tile(entt::registry & entities, entt::entity mouse_tile,
                        SDL_Event const & event)
{
    // do nothing if not the right event type
    if (event.type != SDL_MOUSEWHEEL) {
        return;
    }

    // otherwise, rotate the tile
    auto & tile = entities.get<Tile>(mouse_tile);
    if (event.wheel.y > 0) { increment_rotation(tile.rotation); }
    else if (event.wheel.y < 0) { decrement_rotation(tile.rotation); }
}

void pipes::place_tile(entt::registry & entities, entt::entity mouse_tile,
                       SDL_Event const & event)
{
    // do nothing if not the right event type
    if (event.type != SDL_MOUSEBUTTONUP ||
            event.button.button != SDL_BUTTON_LEFT) {
        return;
    }

    // the grid point closest to the position of the mouse when it was clicked
    auto p = _world_space.nearest_point(event.button.x, event.button.y);

    // do nothing if the tile is already placed
    if (_placed_tiles.find(p) != _placed_tiles.end()) {
        return;
    }

    auto & tile = entities.get<Tile>(mouse_tile);
    tiles::make(entities, tile.name, tile.rotation, p.x, p.y);

    // keep track of the positions that have been placed
    _placed_tiles.insert(p);

    // generate a new random tile
    tile.name = tiles::random_name(_rng);
    tile.rotation = tiles::random_rotation(_rng);
}

