#include "Pipes/Hand.hpp"
#include "Pipes/Tile/Tile.hpp"

Pipes::Hand::Hand(Board & board)
    : board{ &board }
{
}

void Pipes::Hand::on_cursor_moved(int x, int y)
{
    const SDL_Point mouse = board->nearest_point(x, y);
    const SDL_Color color = board->has_adjacent_tile(mouse.x, mouse.y)
                          ? board->tile_settings.placeable_color
                          : board->tile_settings.distant_color;

    if (not current_tile and cached_tile and not board->has_tile(mouse))
    {
        current_tile = pop_cache();
    }

    if (current_tile)
    {
        const SDL_Point position = current_tile->position();
        if (board->has_tile(position))
        {
            cached_tile = { current_tile->name(), current_tile->rotation() };
            current_tile = std::nullopt;
        }
        else
        {
            current_tile->position(mouse);
            current_tile->color() = color;
        }
    }
}

void Pipes::Hand::on_cursor_scrolled(int dy)
{
    if (current_tile)
    {
        if (dy > 0) { ++current_tile->rotation(); }
        else if (dy < 0) { --current_tile->rotation(); }
    }
}

Pipes::TileHandle Pipes::Hand::pop_cache()
{
    auto tile = TileHandle(board->entities);
    if (cached_tile)
    {
        tile.name() = cached_tile->name;
        tile.rotation() = cached_tile->rotation;
        cached_tile = std::nullopt;
    }
    return tile;
}
