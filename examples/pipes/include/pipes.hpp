#pragma once

#include "entities/tile.hpp"
#include "systems/grid.hpp"

#include <ion/ion.hpp>
#include <SDL2/SDL.h>
#include <entt.hpp>

#include <unordered_set>
#include <random>

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

    inline SDL_Color static_bg_color() const
    {
        return SDL_Color{0x42, 0x87, 0xf5, 0xff};
    }
    inline SDL_Color good_bg_color() const
    {
        return SDL_Color{0x9d, 0xbe, 0xf5, 0xff};
    }
    inline SDL_Color distant_bg_color() const
    {
        return SDL_Color{0xd3, 0xd3, 0xd3, 0xff};
    }
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
