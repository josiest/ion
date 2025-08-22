#include "Pipes/TileDeck.hpp"

bool Pipes::TileDeck::is_empty() const
{
    return tile_stack.empty();
}

Pipes::TileID Pipes::TileDeck::next_tile()
{
    TileID tile = tile_stack.back();
    tile_stack.pop_back();
    return tile;
}