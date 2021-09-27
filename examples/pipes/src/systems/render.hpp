#pragma once
#include "types/tile.hpp"
#include "types/components.hpp"

#include "systems/point.hpp"
#include "systems/grid.hpp"
#include "systems/tile.hpp"

#include "entities/tile.hpp"

#include <ion/window/resource.hpp>
#include <entt/entity/registry.hpp>
#include <SDL.h>

namespace systems {

void render(ion::window_resource auto & window,
            systems::grid const & world_space, entt::registry & entities,
            prefab::tile & tile_prefab, pointset const & placed_tiles,
            entt::entity mouse_tile)
{
    // clear the screen
    auto screen = SDL_GetWindowSurface(window);
    SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0, 0, 0));

    // draw the tiles and update the window
    render_static_tiles(window, world_space, entities, tile_prefab);
    render_mouse_tile(window, world_space, entities, tile_prefab,
                      placed_tiles, mouse_tile);
    SDL_UpdateWindowSurface(window);
}

void render_static_tiles(ion::window_resource auto & window,
                         systems::grid const & world_space,
                         entt::registry & entities,
                         prefab::tile & tile_prefab)
{
    auto screen = SDL_GetWindowSurface(window);
    auto static_tiles = entities.view<component::tile, component::position,
                                      component::static_tile>();

    auto const & color = tile_prefab.static_color();
    auto const bg_color = SDL_MapRGB(screen->format, color.r, color.g, color.b);

    static_tiles.each([&tile_prefab, &world_space, screen, bg_color]
            (auto const & tile, auto const & p) {

        // get the sdl surface to render from and the grid sqaure to render to
        auto & tile_surface = tile_prefab.at(tile.name, tile.rotation);
        SDL_Rect grid_square = world_space.unit_square(p.x, p.y);

        // color the background
        SDL_FillRect(screen, &grid_square, bg_color);

        // draw the tile
        SDL_BlitScaled(tile_surface, nullptr, screen, &grid_square);
    });
}

void render_mouse_tile(ion::window_resource auto & window,
                       systems::grid const & world_space,
                       entt::registry & entities, prefab::tile & tile_prefab,
                       pointset const & placed_tiles, entt::entity mouse_tile)
{
    auto const & p = entities.get<component::position>(mouse_tile);
    // do nothing if a static tile already exists at p
    if (placed_tiles.contains({p.x, p.y})) { return; }
    // otherwise get the grid square
    auto grid_square = world_space.unit_square(p.x, p.y);

    // then the bitmap surface
    auto const & tile = entities.get<component::tile>(mouse_tile);
    auto & tile_surface = tile_prefab.at(tile.name, tile.rotation);

    // get the appropriate color: is the mouse next to an already placed tile?
    auto color = tile_prefab.placeable_color();
    if (not systems::is_adjacent(placed_tiles, p)) {
        color = tile_prefab.distant_color();
    }
    // finally, render
    auto screen = SDL_GetWindowSurface(window);
    auto const bg_color = SDL_MapRGB(screen->format, color.r, color.g, color.b);

    // color the background
    SDL_FillRect(screen, &grid_square, bg_color);

    // draw the tile
    SDL_BlitScaled(tile_surface, nullptr, screen, &grid_square);
}

}
