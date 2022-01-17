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
class pipes : public ion::isotope {
public:
    using engine_t = std::mt19937;

    pipes(std::uint32_t width, std::uint32_t height);
    void run();
private:
    // events
    ion::event_system _events;

    // initialize sdl before other sdl resources
    ion::sdl_context _sdl;
    ion::software_renderer _window;
    systems::grid _world_space;

    // ecs
    engine_t _rng;
    entt::registry _entities;
    prefab::tile _tile_prefab;
    pointset _placed_tiles;
};
