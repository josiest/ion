#pragma once
#include "pipes.hpp"
#include "types/tile.hpp"
#include "types/components.hpp"

#include "systems/grid.hpp"
#include "systems/tile.hpp"

#include "entities/tile.hpp"

#include <ion/window/resource.hpp>
#include <entt/entity/registry.hpp>
#include <SDL.h>

void render(ion::window_resource auto & window, entt::registry & entities,
            prefab::tile & tile_prefab,
            entt::entity mouse_tile, pipes & game)
{
    // clear the screen
    auto screen = SDL_GetWindowSurface(window);
    SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0, 0, 0));

    // draw the tiles and update the window
    render_static_tiles(window, entities, tile_prefab, game);
    render_mouse_tile(window, entities, tile_prefab, mouse_tile, game);
    SDL_UpdateWindowSurface(window);
}

void render_static_tiles(ion::window_resource auto & window,
                         entt::registry & entities,
                         prefab::tile & tile_prefab, pipes & game)
{
    auto screen = SDL_GetWindowSurface(window);
    auto static_tiles = entities.view<component::tile, component::position,
                                      component::static_tile>();

    static_tiles.each([&game, &tile_prefab, screen]
            (auto const & tile, auto const & pos) {

        // get the sdl surface to render from and the grid sqaure to render to
        auto & tile_surface = tile_prefab.at(tile.name, tile.rotation);
        SDL_Rect grid_square = game.world_space().unit_square(pos.x, pos.y);

        // render the tile
        auto const & color = tile_prefab.static_color();
        auto color_fmt = SDL_MapRGB(screen->format, color.r, color.g, color.b);
        SDL_FillRect(screen, &grid_square, color_fmt);
        SDL_BlitScaled(tile_surface, nullptr, screen, &grid_square);
    });
}

void render_mouse_tile(ion::window_resource auto & window,
                       entt::registry & entities, prefab::tile & tile_prefab,
                       entt::entity mouse_tile, pipes & game)
{
    auto const & p = entities.get<component::position>(mouse_tile);
    // do nothing if a static tile already exists at p
    if (game.placed_tiles().contains(SDL_Point{p.x, p.y})) { return; }
    // otherwise get the grid square
    auto grid_square = game.world_space().unit_square(p.x, p.y);

    // then the bitmap surface
    auto const & tile = entities.get<component::tile>(mouse_tile);
    auto & tile_surface = tile_prefab.at(tile.name, tile.rotation);

    // get the appropriate color: is the mouse next to an already placed tile?
    auto color = tile_prefab.placeable_color();
    if (not systems::is_adjacent(game.placed_tiles(), p)) {
        color = tile_prefab.distant_color();
    }
    // finally render
    auto screen = SDL_GetWindowSurface(window);
    auto const color_fmt = SDL_MapRGB(screen->format, color.r, color.g, color.b);
    SDL_FillRect(screen, &grid_square, color_fmt);

    SDL_BlitScaled(tile_surface, nullptr, screen, &grid_square);
}
