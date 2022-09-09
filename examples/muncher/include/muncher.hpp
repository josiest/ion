#pragma once

#include "entities/player.hpp"
#include "entities/munchable.hpp"

#include <ion/ion.hpp>
#include <SDL2/SDL_events.h>
#include <entt/entity/registry.hpp>

#include <string>
#include <cstdint>
#include <random>

/**
 * A manager class for game resources
 */
class muncher : public ion::isotope {
public:
    // delete any unwanted implicit constructors
    muncher() = delete;
    muncher(muncher const &) = delete;

    // resource management
    muncher(muncher && temp) = default;

    /**
     * The type of the random engine used
     */
    using engine_t = std::mt19937;

    /**
     * Create a muncher game
     *
     * \param width the width of the game window
     * \param height the height of the game window
     */
    static tl::expected<muncher, std::string>
    with_dimensions(std::uint32_t width, std::uint32_t height);

    /**
     * Run the game
     */
    void run() noexcept;

    /**
     * Reset the game
     */
    void reset() noexcept;
private:
    muncher(ion::system && sys, std::uint32_t width, std::uint32_t height);

    // sdl resources

    // it's important to put the sdl context before any other sdl resources
    ion::system _system;
    ion::hardware_renderer _window;

    // events and input
    ion::event_system _events;
    ion::input::keyboard_axis _input;

    // entt setup
    engine_t _rng;
    entt::registry _entities;

    // entity prefabs
    prefab::player _player_settings;
    prefab::munchable _munchable_settings;

    // entity references
    entt::entity _player;
};

void reset_game(SDL_Event const & event);
