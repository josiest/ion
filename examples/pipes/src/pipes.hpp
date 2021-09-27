#pragma once

#include "types/tile.hpp"
#include "types/point.hpp"
#include "systems/grid.hpp"

#include <ion/ion.hpp>
#include <SDL2/SDL.h>
#include <entt/entity/registry.hpp>

#include <cstdint>
#include <memory>
#include <string>

#include <random>
#include <algorithm>

/** A resource manager for all game resources */
class pipes {
public:
    using engine_t = std::mt19937;

    pipes(std::uint32_t width, std::uint32_t height);
    void run();

    /** Determine if the game is not okay to play */
    inline bool operator!() const
    {
        namespace ranges = std::ranges;
        auto tile_is_bad = [](auto const & pair) { return not *pair.second; };
        return not _sdl or not _window or _tiles.empty()
                        or ranges::any_of(_tiles, tile_is_bad);
    }

    /** The error message if the game isn't okay to play */
    std::string const & error() const { return _error; }

    // shared immutable resources
    inline tilemap const & tiles() const { return _tiles; }
    inline grid const & world_space() const { return _world_space; }

    // shared mutable resources
    inline engine_t & rng() { return _rng; }
    inline pointset & placed_tiles() { return _placed_tiles; }
private:
    // events
    ion::event_system _events;

    // initialize sdl before other sdl resources
    ion::sdl_context _sdl;
    ion::blit_window _window;

    // ecs
    entt::registry _entities;

    // shared immutable resources
    tilemap _tiles;
    grid _world_space;

    // shared mutable resources
    engine_t _rng;
    pointset _placed_tiles;

    // error handling
    std::string _error;
};
