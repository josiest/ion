#include "muncher.hpp"

#include "entities/player.hpp"
#include "entities/munchable.hpp"

#include "systems/render.hpp"
#include "systems/physics.hpp"

#include <ion/ion.hpp>
#include <entt/entity/registry.hpp>
#include <SDL2/SDL.h>

#include <iostream>
#include <string>
#include <cstdint>
#include <random>

muncher & get_game() noexcept
{
    static muncher game{800, 600};
    return game;
}

int main()
{
    auto & game = get_game();

    // crash if the game failed to initialize properly
    if (not game.is_ok()) {
        std::cout << game.error() << std::endl;
    }
    // otherwise run the game
    else {
        game.run();
    }
}

muncher::muncher(std::uint32_t width, std::uint32_t height) noexcept

      // intialize the window with specified dimensions
    : _window{"Muncher", width, height},

      // create a wasd keyboard input axis
      _input(_events, SDLK_d, SDLK_a, SDLK_w, SDLK_s),

      // initialize the random engine with a pseudo-random seed
      _rng{std::random_device{}()}, 

      // initialize the player prefab and create the player entity
      _player_settings{width, height},
      _player{_player_settings.create(_entities)},

      // initialize the munchable prefab
      _munchables{width, height, _rng}, _munchtime_likelihood{.01}
{
    // quit when the user exits the window
    _events.subscribe(SDL_QUIT, &ion::input::quit_on_event);

    // reset the game when the user presses the reset key
    _events.subscribe(SDL_KEYUP, &reset_game);
}

void muncher::run() noexcept
{
    // timer for physics
    ion::clock clock;

    // decider for making a munchable
    std::binomial_distribution<bool> is_munch_time(1, _munchtime_likelihood);

    // run the program
    while (!ion::input::has_quit()) {
        // handle events
        _events.process_queue();

        // apply systems

        // decide if this is the frame to make a new munchable
        if (is_munch_time(_rng)) {
            _munchables.make_munchable(_entities, _player);
        }
        // physics systems
        float const dt = clock.tick();
        accelerate_player(_entities, _player, _input, dt);
        move_munchies(_entities, dt);

        // mechanic systems
        devour(_entities, _player);
        _munchables.filter(_entities);

        // render
        render(_window, _entities);
    }
}

void muncher::reset() noexcept
{
    // destroy the player if they exist
    if (_entities.valid(_player)) {
        _entities.destroy(_player);
    }
    // then reinstantiate them with default settings
    _player = _player_settings.create(_entities);
}

void reset_game(SDL_Event const & event)
{
    // do nothing if not the right event or not the right key
    if (event.type != SDL_KEYUP || event.key.keysym.sym != SDLK_r) {
        return;
    }
    get_game().reset();
}
