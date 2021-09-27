#pragma once
#include "pipes.hpp"
#include "types/tile.hpp"
#include "types/components.hpp"

#include "systems/grid.hpp"
#include "systems/tile.hpp"
#include "systems/resource.hpp"

#include "entities/tile.hpp"

#include <ion/window/resource.hpp>
#include <SDL2/SDL_surface.h>
#include <entt/entity/registry.hpp>

void render(ion::window_resource auto & window, entt::registry & entities,
            entt::entity mouse_tile, pipes & game)
{
    // clear the screen
    auto screen = SDL_GetWindowSurface(window);
    SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0, 0, 0));

    // draw the tiles and update the window
    render_static_tiles(window, entities, game);
    render_mouse_tile(window, entities, mouse_tile, game);
    SDL_UpdateWindowSurface(window);
}

void render_static_tiles(ion::window_resource auto & window,
                         entt::registry & entities, pipes & game)
{
    auto screen = SDL_GetWindowSurface(window);
    auto static_tiles = entities.view<component::tile, component::position,
                                      component::static_tile>();

    static_tiles.each([&game, screen](auto const & tile, auto const & pos) {

        // get the sdl surface to render from and the grid sqaure to render to
        auto tile_surface =
            tiles::get_surface(game.tiles(), tile.name, tile.rotation);

        SDL_Rect grid_square = game.world_space().unit_square(pos.x, pos.y);

        // render the tile
        auto const & color = tiles::static_bg_color;
        SDL_FillRect(screen, &grid_square,
                     SDL_MapRGB(screen->format, color.r, color.g, color.b));
        SDL_BlitScaled(tile_surface, nullptr, screen, &grid_square);
    });
}

void render_mouse_tile(ion::window_resource auto & window,
                       entt::registry & entities, entt::entity mouse_tile,
                       pipes & game)
{
    auto const & p = entities.get<component::position>(mouse_tile);
    // do nothing if a static tile already exists at p
    if (game.placed_tiles().contains(SDL_Point{p.x, p.y})) { return; }
    // otherwise get the grid square
    auto grid_square = game.world_space().unit_square(p.x, p.y);

    // then the bitmap surface
    auto const & tile = entities.get<component::tile>(mouse_tile);
    auto tile_surface =
        tiles::get_surface(game.tiles(), tile.name, tile.rotation);

    // finally render
    auto screen = SDL_GetWindowSurface(window);

    SDL_Color const * color = &tiles::placeable_bg_color;
    if (!tiles::is_adjacent(game.placed_tiles(), p.x, p.y)) {
        color = &tiles::distant_bg_color;
    }
    SDL_FillRect(screen, &grid_square,
                 SDL_MapRGB(screen->format, color->r, color->g, color->b));

    SDL_BlitScaled(tile_surface, nullptr, screen, &grid_square);
}
