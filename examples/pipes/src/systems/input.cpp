#include "pipes.hpp"
#include "types/components.hpp"
#include "entities/tile.hpp"
#include "systems/tile.hpp"

#include <entt/entity/registry.hpp>
#include <SDL2/SDL.h>

void bind_to_mouse(entt::registry & entities, entt::entity tile,
                   grid const & world_space)
{
    // get the mouse coordinates and translate them to grid-space
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);
    auto const q = world_space.nearest_point(mouse);

    // update the desired entity to correspond to the mouse position
    tiles::move(entities, tile, q.x, q.y);
}

void rotate_tile(entt::registry & entities, entt::entity mouse_tile,
                 SDL_Event const & event)
{
    // do nothing if not the right event type
    if (event.type != SDL_MOUSEWHEEL) {
        return;
    }

    // otherwise, rotate the tile
    auto & tile = entities.get<component::tile>(mouse_tile);
    if (event.wheel.y > 0) { tiles::increment_rotation(tile.rotation); }
    else if (event.wheel.y < 0) { tiles::decrement_rotation(tile.rotation); }
}

void place_tile(entt::registry & entities, prefab::tile & tile_prefab,
                pointset & placed_tiles, entt::entity mouse_tile,
                pipes & game, SDL_Event const & event)
{
    // do nothing if not the right event type
    if (event.type != SDL_MOUSEBUTTONUP ||
            event.button.button != SDL_BUTTON_LEFT) {
        return;
    }

    // the grid point closest to the position of the mouse when it was clicked
    auto p = game.world_space().nearest_point(event.button.x, event.button.y);

    // do nothing if the tile is already placed or if no adjacent tiles
    if (placed_tiles.contains(p) || !tiles::is_adjacent(placed_tiles, p.x, p.y)) {
        return;
    }
    // keep track of the positions that have been placed
    tile_prefab.static_copy(entities, placed_tiles, mouse_tile);

    // generate a new random tile
    auto & tile = entities.get<component::tile>(mouse_tile);
    tile.name = tiles::random_name(game.rng());
    tile.rotation = tiles::random_rotation(game.rng());
}

