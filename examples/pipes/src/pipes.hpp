#pragma once

#include "types/components.hpp"
#include "types/tile.hpp"
#include "systems/point.hpp"
#include "systems/grid.hpp"
#include "entities/tile.hpp"

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
    inline bool operator!() const { return not _error.empty(); }
    /** The error message if the game isn't okay to play */
    std::string const & error() const { return _error; }

    // shared immutable resources
    inline systems::grid const & world_space() const { return _world_space; }

    // shared mutable resources
    inline engine_t & rng() { return _rng; }
    inline pointset & placed_tiles() { return _placed_tiles; }
private:
    // events
    ion::event_system _events;

    // initialize sdl before other sdl resources
    ion::sdl_context _sdl;
    ion::blit_window _window;
    systems::grid _world_space;

    // ecs
    engine_t _rng;
    entt::registry _entities;
    prefab::tile _tile_prefab;
    pointset _placed_tiles;

    // error handling
    std::string _error;
};
