#pragma once

#include "types/tile.hpp"
#include "types/point.hpp"
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
    ~pipes();
    void run();

    // shared immutable resources
    inline tilemap const & tiles() const noexcept { return _tiles; }
    inline grid const & world_space() const noexcept { return _world_space; }

    // shared mutable resources
    inline engine_t & rng() noexcept { return _rng; }
    inline pointset & placed_tiles() noexcept { return _placed_tiles; }
private:
    size_t _width, _height;
    ion::event_system _events;

    // shared immutable resources
    tilemap _tiles;
    grid _world_space;

    // shared mutable resources
    engine_t _rng;
    pointset _placed_tiles;
};
