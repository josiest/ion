#pragma once

#include "types/tile.hpp"
#include "types/point.hpp"
#include "systems/grid.hpp"

#include <ion/ion.hpp>
#include <SDL2/SDL.h>
#include <entt.hpp>

#include <random>
#include <cstdint>
#include <memory>

#include <algorithm>

class pipes {
public:
    using engine_t = std::mt19937;

    pipes(std::uint32_t width, std::uint32_t height);
    void run();

    inline bool bad() const noexcept
    {
        namespace ranges = std::ranges;
        auto tile_is_bad = [](auto const & pair) { return not *pair.second; };
        return not _sdl or not _window or _tiles.empty()
                        or ranges::any_of(_tiles, tile_is_bad);
    }

    // shared immutable resources
    inline tilemap const & tiles() const noexcept { return _tiles; }
    inline grid const & world_space() const noexcept { return _world_space; }

    // shared mutable resources
    inline engine_t & rng() noexcept { return _rng; }
    inline pointset & placed_tiles() noexcept { return _placed_tiles; }
private:
    // initialize sdl before other sdl resources
    ion::sdl_context _sdl;
    ion::blit_window _window;

    std::uint32_t _width, _height;
    ion::event_system _events;

    // shared immutable resources
    tilemap _tiles;
    grid _world_space;

    // shared mutable resources
    engine_t _rng;
    pointset _placed_tiles;
};
