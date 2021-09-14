#include "systems/render.hpp"
#include "systems/grid.hpp"

#include "pipes.hpp"
#include "components.hpp"
#include "entities/tile.hpp"

#include <SDL2/SDL_surface.h>
#include <entt.hpp>

void render(ion::window & window, entt::registry & entities, pipes & game)
{
    // clear the screen
    auto screen = window.sdl_surface();
    SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0, 0, 0));

    // draw the tiles and update the window
    render_grid_tiles(window, entities, game);
    SDL_UpdateWindowSurface(window.sdl_window());
}

void render_grid_tiles(ion::window & window, entt::registry & entities,
                       pipes & game)
{
    auto screen = window.sdl_surface();
    auto view = entities.view<Tile, Position>();
    view.each([&game, screen](auto const & tile, auto const & pos) {

        // get the sdl surface to render from and the grid sqaure to render to
        auto tile_surface = game.tilemap()
                                .sdl_surface(tile.name, tile.rotation);

        SDL_Rect grid_square = game.world_space()
                                   .unit_square(pos.x, pos.y);

        // render the tile
        SDL_BlitScaled(tile_surface, nullptr, screen, &grid_square);
    });
}
