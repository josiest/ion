#pragma once
#include "components.hpp"

#include "systems/point.hpp"
#include "systems/grid.hpp"

#include "entities/tile.hpp"

#include <ion/ion.hpp>
#include <entt/entt.hpp>
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
                         const grid & world_space,
                         entt::registry & entities,
                         prefab::tile & tile_prefab)
{
    SDL_Surface * screen = SDL_GetWindowSurface(window);

    // get the background color as uint to pass to FillRect
    const SDL_Color & rgb = tile_prefab.static_color();
    const std::uint32_t color = SDL_MapRGB(screen->format, rgb.r, rgb.g, rgb.b);

    entities.view<component::tile, component::position, component::static_tile>()
            .each([&](const auto & tile, const auto & position)
    {
        // get the sdl surface to render from and the grid square to render to
        SDL_Surface * tile_surface = tile_prefab.loaded_tiles.image_for(tile.name, tile.rotation);
        SDL_Rect grid_square = world_space.unit_square(position);

        // color the background and draw the tile
        SDL_FillRect(screen, &grid_square, color);
        SDL_BlitScaled(tile_surface, nullptr, screen, &grid_square);
    });
}

void render_mouse_tile(ion::window_resource auto & window,
                       grid const & world_space,
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
    SDL_Surface * tile_surface = tile_prefab.loaded_tiles.image_for(tile.name, tile.rotation);

    // get the appropriate color: is the mouse next to an already placed tile?
    SDL_Surface * screen = SDL_GetWindowSurface(window);
    const SDL_Color rgb = is_adjacent(placed_tiles, p) ? tile_prefab.placeable_color()
                                                       : tile_prefab.distant_color();
    const std::uint32_t color = SDL_MapRGB(screen->format, rgb.r, rgb.g, rgb.b);

    // finally, render
    SDL_FillRect(screen, &grid_square, color);                      // color the background
    SDL_BlitScaled(tile_surface, nullptr, screen, &grid_square);    // draw the tile
}

}
