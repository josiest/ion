#pragma once
#include <optional>
#include <entt/entity/registry.hpp>
#include "systems/grid.hpp"

#include "Pipes/Tile/TileMap.hpp"
#include "Pipes/TileSettings.hpp"

namespace Pipes
{
class TileHand
{
public:
    TileHand() = delete;
    TileHand(entt::registry & entities, const systems::grid & world_space,
             const TileSettings & tile_settings, const PointSet & placed_tiles);

    void on_cursor_moved(int x, int y);
    void on_cursor_scrolled(int dy);

    std::optional<entt::entity> current_tile = std::nullopt;
protected:
    entt::registry * entities;
    const systems::grid * world_space;
private:
    const TileSettings * tile_settings;
    const PointSet * placed_tiles;

    std::optional<TileID> cached_tile = std::nullopt;
};
}
