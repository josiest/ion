#pragma once

#include "systems.hpp"

#include "Pipes/PointSet.hpp"
#include "Pipes/Board.hpp"
#include "Pipes/Deck.hpp"
#include "Pipes/Hand.hpp"
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

    // events
    ion::event_sink _events;

    // initialize sdl before other sdl resources
    ion::sdl_context _sdl;
    ion::software_renderer _window;

    // ecs
    engine_t _rng;

    // tile
    Pipes::TileMap loaded_tiles;

    Pipes::Board board;
    Pipes::Deck deck;
    Pipes::Hand hand;
};
