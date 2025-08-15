#include "muncher.hpp"

#include "entities/player.hpp"
#include "entities/munchable.hpp"

#include "systems/render.hpp"
#include "systems/physics.hpp"
#include "systems/mechanics.hpp"

#include <ion/ion.hpp>
#include <SDL2/SDL.h>
#include <entt/entity/registry.hpp>

#include <iostream>
#include <string>
#include <cstdint>
#include <random>

muncher & get_game() noexcept
{
    static muncher game{800, 600};
    return game;
}

int main(int argc, char * argv[])
{
    auto & game = get_game();

    // crash if the game failed to initialize properly
    if (not game) {
        std::cout << game.get_error() << std::endl;
        return EXIT_FAILURE;
    }
    // otherwise run the game
    else {
        game.run();
    }
    return EXIT_SUCCESS;
}

void event_sink::poll()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
        case SDL_QUIT:
            quit.publish();
            break;
        case SDL_KEYDOWN:
            key_down.publish(event.key.keysym.sym);
            break;
        case SDL_KEYUP:
            key_up.publish(event.key.keysym.sym);
            break;
        default:
            break;
        }
    }
}

muncher::muncher(std::uint32_t width, std::uint32_t height) noexcept

      // intialize the window with specified dimensions
    : _window{ion::hardware_renderer::basic_window(
          "Muncher", width, height, SDL_WINDOW_RESIZABLE
      )},

      // create a wasd keyboard input axis
      _input(SDLK_d, SDLK_a, SDLK_w, SDLK_s),

      // initialize the random engine with a pseudo-random seed
      _rng{std::random_device{}()}, 

      // initialize the player and munchable prefabs
      _player_settings{_window},
      _munchable_settings{_window},

      // create the player
      _player{_player_settings.create(_entities)}
{
    // check if sdl resources initialized properly
    if (not _sdl) {
        set_error(_sdl.get_error()); return;
    }
    if (not _window) {
        set_error(_window.get_error()); return;
    }
    _events.on_quit().connect<&ion::input::quit>();
    _events.on_key_down().connect<&ion::input::set_key>();
    _events.on_key_up().connect<&ion::input::release_key>();
    _events.on_key_up().connect<&reset_game>();
}

void muncher::run() noexcept
{
    // timer for physics
    ion::clock clock;

    // run the program
    while (not ion::input::has_quit()) {
        // handle events
        _events.poll();

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
        SDL_GetWindowSize(_window, &width, &height);
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

void reset_game(SDL_Keycode keycode)
{
    // do nothing if not the right event or not the right key
    if (keycode == SDLK_r) {
        get_game().reset();
    }
}
