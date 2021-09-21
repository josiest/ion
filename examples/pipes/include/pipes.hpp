#pragma once

#include "types/tile.hpp"
#include "types/point.hpp"
#include "systems/grid.hpp"

#include <ion/ion.hpp>
#include <SDL2/SDL.h>
#include <entt.hpp>

#include <random>
#include <cstdint>

class pipes {
public:
    using engine_t = std::mt19937;

    pipes(std::uint32_t width, std::uint32_t height);
    void run();

    inline bool is_ok() const noexcept { return _error.empty(); }
    inline std::string error() const noexcept { return _error; }

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

    // error handling
    std::string _error;
};
