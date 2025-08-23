#pragma once
#include <optional>
#include <entt/entity/registry.hpp>
#include "Pipes/Board.hpp"
#include "Pipes/Tile/Tile.hpp"

namespace Pipes
{
class Hand
{
public:
    Hand() = delete;
    Hand(Board & board);

    void on_cursor_moved(int x, int y);
    void on_cursor_scrolled(int dy);

    std::optional<TileHandle> current_tile = std::nullopt;
private:
    Board * board;
    std::optional<TileID> cached_tile = std::nullopt;

    TileHandle pop_cache();
};
}
