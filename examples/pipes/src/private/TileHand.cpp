#include "Pipes/TileHand.hpp"
#include "Pipes/Tile/Tile.hpp"

Pipes::TileHand::TileHand(entt::registry & entities, const systems::grid & world_space,
                          const TileSettings & tile_settings, const pointset & placed_tiles)
    : entities{ &entities },
      world_space{ &world_space },
      tile_settings{ &tile_settings },
      placed_tiles{ &placed_tiles }
{
}

void Pipes::TileHand::on_cursor_moved(int x, int y)
{
    const SDL_Point mouse = world_space->nearest_point(x, y);
    const SDL_Color color = systems::is_adjacent(*placed_tiles, mouse.x, mouse.y)
                          ? tile_settings->placeable_color : tile_settings->distant_color;

    if (not current_tile and cached_tile and not placed_tiles->contains(mouse))
    {
        current_tile = entities->create();
        entities->emplace<component::position>(*current_tile, mouse.x, mouse.y);
        entities->emplace<Component::Tile>(*current_tile, cached_tile->name, cached_tile->rotation, color);
        cached_tile = std::nullopt;
    }

    if (current_tile)
    {
        auto & position = entities->get<component::position>(*current_tile);
        if (placed_tiles->contains(position))
        {
            const auto& tile = entities->get<Pipes::Component::Tile>(*current_tile);
            cached_tile = { tile.name, tile.rotation };
            entities->destroy(*current_tile);
            current_tile = std::nullopt;
        }
        else
        {
            entities->replace<component::position>(*current_tile, mouse.x, mouse.y);
            entities->get<Pipes::Component::Tile>(*current_tile).color = color;
        }
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
