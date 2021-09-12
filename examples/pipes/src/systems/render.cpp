#include "systems/render.hpp"
#include "components.hpp"

#include "tiles.hpp"
#include "grid.hpp"

#include <ion/render/window.hpp>
#include <entt.hpp>

void render_grid_tiles(ion::Window & window, entt::registry & registry,
                       Grid const & grid, tiles::TileMap & tilemap)
{
    auto view = registry.view<Tile, Position>();
    view.each([&window, &tilemap, &grid](auto const & tile, auto const & pos) {

        // get the sdl surface to render from and the grid sqaure to render to
        auto tile_surface = tilemap.sdl_surface(tile.name, tile.rot);
        SDL_Rect grid_square = grid.rect(pos.x, pos.y);

        // render the tile
        SDL_BlitScaled(tile_surface, nullptr, window.sdl_surface(), &grid_square);
    });
}
