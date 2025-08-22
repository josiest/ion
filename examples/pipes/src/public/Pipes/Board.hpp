#pragma once
#include "systems/grid.hpp"
#include "Pipes/TileSettings.hpp"
#include "Pipes/Tile/TileMap.hpp"
#include <entt/entity/registry.hpp>

namespace Pipes
{
class Deck;

class Board
{
public:
    Board(systems::grid world_space, TileMap && loaded_tiles);

    bool has_tile(int x, int y) const;
    bool has_adjacent_tile(int x, int y) const;

    entt::entity draw_from_deck(Deck & deck, const SDL_Point & position);

    void place_tile(entt::entity tile_id);
    void render(SDL_Window * window) const;

    const Pipes::TileSettings tile_settings;

    entt::registry entities;
    const systems::grid world_space;
private:
    const Pipes::TileMap loaded_tiles;
    Pipes::PointSet placed_tiles;
};
}