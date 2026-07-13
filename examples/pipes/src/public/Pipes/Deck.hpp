#pragma once
#include "Pipes/Tile/TileMap.hpp"

namespace Pipes
{
class Deck
{
public:
    Deck() = delete;
    template<typename RandomEngine, std::unsigned_integral Natural>
    // requires RandomNumberEngine<RandomEngine>
    Deck(RandomEngine & rng, Natural num_tiles);

    bool is_empty() const;
    TileID next_tile();
private:
    std::vector<TileID> tile_stack;
};
}

template<typename RandomEngine, std::unsigned_integral Natural>
Pipes::Deck::Deck(RandomEngine & rng, Natural num_tiles)
{
    for (Natural k = 0; k < num_tiles; ++k)
    {
        tile_stack.emplace_back(TileInfo::random_name(rng), TileInfo::random_rotation(rng));
    }
}
