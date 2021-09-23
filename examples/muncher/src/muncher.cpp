#include "muncher.hpp"

#include "entities/player.hpp"
#include "entities/munchable.hpp"

#include "systems/render.hpp"
#include "systems/physics.hpp"
#include "systems/mechanics.hpp"

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
    : _window{"Muncher", width, height, SDL_WINDOW_RESIZABLE},

      // create a wasd keyboard input axis
      _input(_events, SDLK_d, SDLK_a, SDLK_w, SDLK_s),

      // initialize the random engine with a pseudo-random seed
      _rng{std::random_device{}()}, 

      // initialize the player and munchable prefabs
      _player_settings{_window.sdl_window()},
      _munchable_settings{_window.sdl_window()},

      // create the player
      _player{_player_settings.create(_entities)}
{
    // check if sdl resources initialized properly
    if (_sdl.bad()) {
        _error = _sdl.error(); return;
    }
    if (not _window.is_ok()) {
        _error = _window.error(); return;
    }

    // quit when the user exits the window
    _events.subscribe(SDL_QUIT, &ion::input::quit_on_event);

    // reset the game when the user presses the reset key
    _events.subscribe(SDL_KEYUP, &reset_game);
}

void muncher::run() noexcept
{
    // timer for physics
    ion::clock clock;

    // run the program
    while (!ion::input::has_quit()) {
        // handle events
        _events.process_queue();

        // apply systems
        float const dt = clock.tick();

        // create a new munchable entity if it's time to
        if (_munchable_settings.should_munch(dt, _rng)) {

            _munchable_settings.create(_entities, _player,
                                       _player_settings, _rng);
        }
        // physics systems
        systems::accelerate_player(_entities, _player, _input, dt);
        systems::move_munchies(_entities, dt);

        // mechanics systems
        systems::munch(_entities, _player);

        // get the size of the screen to filter out munchables
        int width, height;
        SDL_GetWindowSize(_window.sdl_window(), &width, &height);
        systems::filter_munchables(_entities, width, height);

        // render
        systems::render(_window, _entities);
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
