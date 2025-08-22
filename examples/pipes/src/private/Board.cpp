#include "Pipes/Board.hpp"
#include "Pipes/Tile/Tile.hpp"
#include "Pipes/Deck.hpp"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_video.h>

#include <vector>
#include <algorithm>

Pipes::Board::Board(Pipes::Grid && world_space, TileMap && loaded_tiles)

    : world_space(std::move(world_space)), loaded_tiles(std::move(loaded_tiles))
{
}

bool Pipes::Board::has_tile(int x, int y) const
{
    return placed_tiles.contains({x, y});
}

bool Pipes::Board::has_adjacent_tile(int x, int y) const
{
    // a list of adjacent points
    std::vector<SDL_Point> points{
        {x+1, y}, {x-1, y}, {x, y+1}, {x, y-1}
    };
    // determine if an adjacent tile has been placed
    return std::ranges::any_of(points, [this](auto const & p) {
        return placed_tiles.contains(p);
    });
}

entt::entity Pipes::Board::draw_from_deck(Pipes::Deck & deck, const SDL_Point & position)
{
    const entt::entity next_tile = entities.create();
    entities.emplace<component::position>(next_tile, position.x, position.y);

    const auto [name, rotation] = deck.next_tile();
    const SDL_Color color = has_adjacent_tile(position.x, position.y)
                          ? tile_settings.placeable_color
                          : tile_settings.distant_color;

    entities.emplace<Pipes::Component::Tile>(next_tile, name, rotation, color);
    return next_tile;
}

void Pipes::Board::place_tile(entt::entity tile_id)
{
    const SDL_Point position = static_cast<SDL_Point>(entities.get<component::position>(tile_id));
    entities.get<Pipes::Component::Tile>(tile_id).color = tile_settings.static_color;
    placed_tiles.try_emplace(position, tile_id);
}

void Pipes::Board::render(SDL_Window * window) const
{
    // clear the screen
    auto screen = SDL_GetWindowSurface(window);
    SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0, 0, 0));

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