#pragma once

#include "entities/player.hpp"
#include "entities/munchable.hpp"

#include <ion/ion.hpp>
#include <SDL2/SDL_events.h>
#include <entt/entity/registry.hpp>
#include <entt/signal/sigh.hpp>

#include <string>
#include <cstdint>
#include <random>

class event_sink
{
public:
    void poll();

    inline auto on_quit() { return entt::sink{ quit }; }
    inline auto on_key_down() { return entt::sink{ key_down }; }
    inline auto on_key_up() { return entt::sink{ key_up }; }
protected:
    entt::sigh<void()> quit;
    entt::sigh<void(SDL_Keycode)> key_down;
    entt::sigh<void(SDL_Keycode)> key_up;
};

/**
 * A manager class for game resources
 */
class muncher : public ion::isotope {
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
private:
    // sdl resources

    // it's important to put the sdl context before any other sdl resources
    ion::sdl_context _sdl;
    ion::hardware_renderer _window;

    // events and input
    event_sink _events;
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

muncher & get_game() noexcept;
void reset_game(SDL_Keycode keycode);
