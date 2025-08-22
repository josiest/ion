#pragma once
#include "Pipes/Tile/TileMap.hpp"

namespace Pipes
{
class TileDeck
{
public:
    TileDeck() = delete;
    template<typename RandomEngine, std::unsigned_integral Natural>
    // requires RandomNumberEngine<RandomEngine>
    TileDeck(RandomEngine & rng, Natural num_tiles);

    bool is_empty() const;
    Pipes::TileID next_tile();
private:
    std::vector<Pipes::TileID> tile_stack;
};
}

template<typename RandomEngine, std::unsigned_integral Natural>
Pipes::TileDeck::TileDeck(RandomEngine & rng, Natural num_tiles)
{
    for (Natural k = 0; k < num_tiles; ++k)
    {
        tile_stack.emplace_back(Pipes::TileInfo::random_name(rng), Pipes::TileInfo::random_rotation(rng));
    }
}
