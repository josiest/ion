#pragma once

#include "entities/player.hpp"
#include "entities/munchable.hpp"

#include <SDL3/SDL_events.h>
#include <entt/entity/registry.hpp>
#include <entt/signal/sigh.hpp>

#include <random>
#include <ion/input/axis.hpp>

class event_sink
{
public:
    void poll();

    auto on_quit() { return entt::sink{ quit }; }
    auto on_key_down() { return entt::sink{ key_down }; }
    auto on_key_up() { return entt::sink{ key_up }; }
protected:
    entt::sigh<void()> quit;
    entt::sigh<void(SDL_Keycode)> key_down;
    entt::sigh<void(SDL_Keycode)> key_up;
};

/**
 * A manager class for game resources
 */
class muncher {
public:
    // delete any unwanted implicit constructors
    muncher(muncher const &) = delete;

    /**
     * The type of the random engine used
     */
    using engine_t = std::mt19937;

    /**
     * Create a muncher game
     */
    muncher();

    /**
     * Run the game
     */
    void update(float delta_time);

    /**
     * Reset the game
     */
    void reset();
private:
    // events and input
    ion::input::keyboard_axis _input;

    // entt setup
    engine_t _rng;
    entt::registry _entities;

    // entity prefabs
    prefab::player _player_settings;
    prefab::munchable _munchable_settings;

    // entity references
    entt::entity _player = entt::null;
};

muncher & get_game();
void reset_game(SDL_Keycode keycode);
