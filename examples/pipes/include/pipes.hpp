#pragma once

#include "entities/tile.hpp"
#include "systems/grid.hpp"

#include <ion/ion.hpp>
#include <SDL2/SDL.h>
#include <entt.hpp>

#include <unordered_set>
#include <random>

namespace std {
template<> struct hash<SDL_Point> {
    size_t operator()(SDL_Point const & p) const
    {
        return hash<int>{}(p.x) ^ hash<int>{}(p.y);
    }
};
}
inline bool operator==(SDL_Point const & p, SDL_Point const & q)
{
    return p.x == q.x && p.y == q.y;
}
using pointset = std::unordered_set<SDL_Point>;

class pipes {
public:
    using engine_t = std::mt19937;

    pipes(size_t width, size_t height);
    inline ~pipes() { _tiles.clear(); SDL_Quit(); }
    void run();

    inline tiles::map & tilemap() { return _tiles; }
    inline grid const & world_space() const { return _world_space; }

    inline engine_t & rng() { return _rng; }
    inline pointset & placed_tiles() { return _placed_tiles; }
private:
    size_t _width, _height;
    ion::event_system _events;

    // shared immutable resources
    tiles::map _tiles;
    grid _world_space;

    // shared mutable resources
    engine_t _rng;
    pointset _placed_tiles;
};
