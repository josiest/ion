#include "Pipes/Board.hpp"
#include "Pipes/Tile/Tile.hpp"
#include "Pipes/Deck.hpp"

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>

#include <array>
#include <algorithm>

Pipes::Board::Board(TileMap && loaded_tiles,
                    const TileSettings & tile_settings)

    : tile_settings(tile_settings),
      loaded_tiles(std::move(loaded_tiles))
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

SDL_Point Pipes::Board::nearest_point(int x, int y) const
{
    auto board_position = pixel_to_board * glm::fvec3(x, y, 1.f);
    // pixel_to_board will give us a fractional point, but we want to floor bc units start from bottom left
    board_position.x = std::floor(board_position.x);
    board_position.y = std::floor(board_position.y);
    return { static_cast<int>(board_position.x), static_cast<int>(board_position.y) };
}

SDL_Rect Pipes::Board::unit_square(int x, int y) const
{
    const auto board_to_pixels = transform.global_basis();
    const auto pixel_position = board_to_pixels * glm::fvec3(x, y, 1.f);
    const int unit_size = static_cast<int>(transform.scale.x);

    // subtract unit size from y bc we're drawing from upper left, but coordinates start from bottom left
    return SDL_Rect{ static_cast<int>(pixel_position.x), static_cast<int>(pixel_position.y - unit_size),
                     unit_size, unit_size };
}

void Pipes::Board::render(SDL_Window * window) const
{
    // clear the screen
    auto screen = SDL_GetWindowSurface(window);

    const auto * pixel_format = SDL_GetPixelFormatDetails(screen->format);
    const int bg_color = SDL_MapRGB(pixel_format, nullptr,
                                    background_color.r, background_color.g, background_color.b);

    SDL_FillSurfaceRect(screen, nullptr, bg_color);

    // draw the tiles and update the window
    entities.view<Component::Tile, Component::Position>()
            .each([&](const auto & tile, const auto & position)
    {
        // get the sdl surface to render from and the grid square to render to
        SDL_Surface * tile_surface = loaded_tiles.image_for(tile.name, tile.rotation);
        SDL_Rect grid_square = unit_square(position);

        // color the background and draw the til
        const int fg_color = SDL_MapRGB(pixel_format, nullptr, tile.color.r, tile.color.g, tile.color.b);
        SDL_FillSurfaceRect(screen, &grid_square, fg_color);
        SDL_BlitSurfaceScaled(tile_surface, nullptr, screen, &grid_square, SDL_SCALEMODE_NEAREST);
    });
    SDL_UpdateWindowSurface(window);
}