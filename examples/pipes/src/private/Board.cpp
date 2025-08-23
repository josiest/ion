#include "Pipes/Board.hpp"
#include "Pipes/Tile/Tile.hpp"
#include "Pipes/Deck.hpp"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_video.h>

#include <array>
#include <algorithm>

Pipes::Board::Board(Pipes::Grid && world_space,
                    TileMap && loaded_tiles,
                    const TileSettings & tile_settings)

    : world_space(std::move(world_space)),
      loaded_tiles(std::move(loaded_tiles)),
      tile_settings(tile_settings)
{
}

bool Pipes::Board::has_tile(int x, int y) const
{
    return placed_tiles.contains({x, y});
}

bool Pipes::Board::has_adjacent_tile(int x, int y) const
{
    // a list of adjacent points
    std::array<SDL_Point, 4> points{{
        {x+1, y}, {x-1, y}, {x, y+1}, {x, y-1}
    }};
    // determine if an adjacent tile has been placed
    return std::ranges::any_of(points, [this](auto const & p) {
        return placed_tiles.contains(p);
    });
}

Pipes::TileHandle Pipes::Board::draw_from(Pipes::Deck & deck, const SDL_Point & position)
{
    auto tile = TileHandle(entities);
    tile.position(position);

    const auto [name, rotation] = deck.next_tile();
    tile.name() = name;
    tile.rotation() = rotation;
    tile.color() = has_adjacent_tile(position)? tile_settings.placeable_color
                                              : tile_settings.distant_color;

    return tile;
}

void Pipes::Board::place_tile(Pipes::TileHandle && tile)
{
    tile.color() = tile_settings.static_color;
    const SDL_Point position = tile.position();
    placed_tiles.try_emplace(position, std::move(tile));
}

void Pipes::Board::render(SDL_Window * window) const
{
    // clear the screen
    auto screen = SDL_GetWindowSurface(window);
    SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format,
                                             background_color.r, background_color.g, background_color.b));

    // draw the tiles and update the window
    entities.view<Component::Tile, component::position>()
            .each([this, screen](const auto & tile, const auto & position)
    {
        // get the sdl surface to render from and the grid square to render to
        SDL_Surface * tile_surface = loaded_tiles.image_for(tile.name, tile.rotation);
        SDL_Rect grid_square = world_space.unit_square(position);

        // color the background and draw the tile
        SDL_FillRect(screen, &grid_square, SDL_MapRGB(screen->format, tile.color.r, tile.color.g, tile.color.b));
        SDL_BlitScaled(tile_surface, nullptr, screen, &grid_square);
    });
    SDL_UpdateWindowSurface(window);
}