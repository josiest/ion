#pragma once
#include "Pipes/Grid.hpp"
#include "Pipes/PointSet.hpp"
#include "Pipes/TileSettings.hpp"
#include "Pipes/Tile/TileMap.hpp"
#include <entt/entity/registry.hpp>

struct SDL_Window;

namespace Pipes
{
class Deck;

class Board
{
public:
    Board(Grid && world_space, TileMap && loaded_tiles);

    bool has_tile(int x, int y) const;
    bool has_adjacent_tile(int x, int y) const;

    entt::entity draw_from_deck(Deck & deck, const SDL_Point & position);

    void place_tile(entt::entity tile_id);
    void render(SDL_Window * window) const;

    const TileSettings tile_settings;

    entt::registry entities;
    const Grid world_space;
private:
    const TileMap loaded_tiles;
    PointSet placed_tiles;
};
}