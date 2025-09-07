#pragma once
#include "Pipes/Grid.hpp"
#include "Pipes/PointSet.hpp"
#include "Pipes/Tile.hpp"
#include "ion/transform.hpp"
#include <entt/entity/registry.hpp>

struct SDL_Window;

namespace Pipes
{
class Deck;

class Board
{
public:
    Board(TileMap && loaded_tiles,
          const TileSettings & tile_settings = TileSettings{});

    bool has_tile(int x, int y) const;
    bool has_tile(const Point auto & p) const;

    bool has_adjacent_tile(int x, int y) const;
    bool has_adjacent_tile(const Point auto & p) const;

    TileHandle draw_from(Deck & deck, const SDL_Point & position);
    void place_tile(TileHandle && tile);

    SDL_Point nearest_point(int x, int y) const;
    SDL_Point nearest_point(const Point auto & p) const;

    SDL_Rect unit_square(int x, int y) const;
    SDL_Rect unit_square(const Point auto & p) const;
    void render(SDL_Window * window) const;

    const TileSettings tile_settings;

    entt::registry entities;
    ion::transform2f transform;
    glm::mat3x3 pixel_to_board;

    SDL_Color background_color{ 0x0, 0x0, 0x0, 0xff };
private:
    const TileMap loaded_tiles;
    PointSet placed_tiles;
};
}

bool Pipes::Board::has_tile(const Point auto & p) const
{
    return has_tile(p.x, p.y);
}

bool Pipes::Board::has_adjacent_tile(const Point auto & p) const
{
    return has_adjacent_tile(p.x, p.y);
}

SDL_Point Pipes::Board::nearest_point(const Point auto & p) const
{
    return nearest_point(p.x, p.y);
}

SDL_Rect Pipes::Board::unit_square(const Point auto & p) const
{
    return unit_square(p.x, p.y);
}
