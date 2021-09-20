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
class muncher {
public:
    // delete any unwanted implicit constructors
    muncher() = delete;
    muncher(muncher const &) = delete;

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
    muncher(std::uint32_t width, std::uint32_t height) noexcept;

    /**
     * Run the game
     */
    void run() noexcept;

    /**
     * Reset the game
     */
    void reset() noexcept;

    /**
     * Determine if the game is in a valid state to run
     */
    inline bool is_ok() const noexcept
    {
        return _error.empty();
    }

    /**
     * Get the error message explaining why the game can't run
     */
    inline std::string error() const noexcept
    {
        return _error;
    }

    /**
     * Set the error message that explains why the game can't run
     */
    inline void error(std::string const & message) noexcept
    {
        _error = message;
    }
private:
    // sdl resources

    // it's important to put the sdl context before any other sdl resources
    ion::sdl_context _sdl;
    ion::render_window _window;

    // events and input
    ion::event_system _events;
    ion::input::keyboard_axis _input;

    // entt setup
    engine_t _rng;
    entt::registry _entities;

    // entity references and managers
    prefab::player _player_settings;
    entt::entity _player;

    munchable_factory _munchables;
    double _munchtime_likelihood;

    // error handling
    std::string _error;
};

muncher & get_game() noexcept;
void reset_game(SDL_Event const & event);
