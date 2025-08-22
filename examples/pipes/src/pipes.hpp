#pragma once

#include "systems.hpp"

#include "Pipes/TileDeck.hpp"
#include "Pipes/TileHand.hpp"
#include "Pipes/TileSettings.hpp"

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
    void on_mouse_clicked();

    entt::entity next_tile_from_deck(const SDL_Point& position);
    void place_tile(entt::entity tile_id);

    // events
    ion::event_sink _events;

    // initialize sdl before other sdl resources
    ion::sdl_context _sdl;
    ion::software_renderer _window;
    systems::grid _world_space;

    // ecs
    engine_t _rng;
    entt::registry _entities;
    pointset _placed_tiles;

    // tile
    Pipes::TileSettings tile_settings;
    Pipes::TileMap loaded_tiles;

    Pipes::TileDeck deck;
    Pipes::TileHand hand;
};
