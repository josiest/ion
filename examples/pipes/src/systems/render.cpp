#include "components.hpp"

#include "entities/tile.hpp"

#include "systems/grid.hpp"
#include "systems/render.hpp"

#include <SDL2/SDL_surface.h>
#include <entt.hpp>

void render_grid_tiles(SDL_Surface * screen, grid const & world_space,
                       entt::registry & entities, tiles::map & tilemap)
{
    auto view = entities.view<Tile, Position>();
    view.each([screen, &world_space, &tilemap](auto const & tile, auto const & pos) {

        // get the sdl surface to render from and the grid sqaure to render to
        auto tile_surface = tilemap.sdl_surface(tile.name, tile.rotation);
        SDL_Rect grid_square = world_space.unit_square(pos.x, pos.y);

        // render the tile
        SDL_BlitScaled(tile_surface, nullptr, screen, &grid_square);
    });
}
