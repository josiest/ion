#pragma once

#include "systems.hpp"
#include "entities.hpp"

#include <ion/ion.hpp>
#include <entt/entt.hpp>

#include <cstdint>
#include <random>

/** A resource manager for all game resources */
class pipes : public ion::isotope {
public:
    using engine_t = std::mt19937;

    pipes(std::uint32_t width, std::uint32_t height);
    void run();
private:
    void rotate_tile(int dy);
    void place_tile();

    // events
    ion::event_sink _events;

    // initialize sdl before other sdl resources
    ion::sdl_context _sdl;
    ion::software_renderer _window;
    systems::grid _world_space;

    // ecs
    engine_t _rng;
    entt::registry _entities;
    prefab::tile _tile_prefab;
    pointset _placed_tiles;

    // tile
    std::optional<entt::entity> _current_tile;
};
