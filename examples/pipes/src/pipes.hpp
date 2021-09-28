#pragma once

#include "components.hpp"
#include "systems.hpp"
#include "entities.hpp"

#include <ion/ion.hpp>
#include <entt/entt.hpp>
#include <SDL.h>

#include <string>
#include <cstdint>
#include <random>

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
