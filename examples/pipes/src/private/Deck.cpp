#include "Pipes/Deck.hpp"

bool Pipes::Deck::is_empty() const
{
    return tile_stack.empty();
}

Pipes::TileID Pipes::Deck::next_tile()
{
    TileID tile = tile_stack.back();
    tile_stack.pop_back();
    return tile;
}