#include "Pipes/TileHand.hpp"
#include "Pipes/Tile/Tile.hpp"

Pipes::TileHand::TileHand(entt::registry & entities, const systems::grid & world_space)
    : entities{ &entities }, world_space{ &world_space }
{
}

void Pipes::TileHand::on_cursor_moved(int x, int y)
{
    if (current_tile)
    {
        auto & position = entities->get<component::position>(*current_tile);
        position = world_space->nearest_point(x, y);
    }
}

void Pipes::TileHand::on_cursor_scrolled(int dy)
{
    if (current_tile)
    {
        auto & tile = entities->get<Pipes::Component::Tile>(*current_tile);
        if (dy > 0) { ++tile.rotation; }
        else if (dy < 0) { --tile.rotation; }
    }
}
