#include "Pipes/Hand.hpp"
#include "Pipes/Tile/Tile.hpp"

Pipes::Hand::Hand(Board & board)
    : board{ &board }
{
}

void Pipes::Hand::on_cursor_moved(int x, int y)
{
    const SDL_Point mouse = board->world_space.nearest_point(x, y);
    const SDL_Color color = board->has_adjacent_tile(mouse.x, mouse.y)
                          ? board->tile_settings.placeable_color
                          : board->tile_settings.distant_color;

    if (not current_tile and cached_tile and not board->has_tile(mouse.x, mouse.y))
    {
        current_tile = board->entities.create();
        board->entities.emplace<component::position>(*current_tile, mouse.x, mouse.y);
        board->entities.emplace<Component::Tile>(*current_tile, cached_tile->name, cached_tile->rotation, color);
        cached_tile = std::nullopt;
    }

    if (current_tile)
    {
        auto & position = board->entities.get<component::position>(*current_tile);
        if (board->has_tile(position.x, position.y))
        {
            const auto& tile = board->entities.get<Pipes::Component::Tile>(*current_tile);
            cached_tile = { tile.name, tile.rotation };
            board->entities.destroy(*current_tile);
            current_tile = std::nullopt;
        }
        else
        {
            board->entities.replace<component::position>(*current_tile, mouse.x, mouse.y);
            board->entities.get<Pipes::Component::Tile>(*current_tile).color = color;
        }
    }
}

void Pipes::Hand::on_cursor_scrolled(int dy)
{
    if (current_tile)
    {
        auto & tile = board->entities.get<Pipes::Component::Tile>(*current_tile);
        if (dy > 0) { ++tile.rotation; }
        else if (dy < 0) { --tile.rotation; }
    }
}
