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

int main()
{
    auto game_result = muncher::with_dimensions(800, 600);
    if (not game_result) {
        std::cerr << game_result.error() << "\n";
        return EXIT_FAILURE;
    }
    auto game = *std::move(game_result);

    // crash if the game failed to initialize properly
    if (not game) {
        std::cout << game.get_error() << std::endl;
    }
    // otherwise run the game
    else {
        game.run();
    }
}

tl::expected<ion::window, std::string>
muncher::with_dimensions(std::uint32_t width, std::uint32_t height)
{
    auto window_result = ion::window::at_anywhere(
            "Muncher", width, height, SDL_WINDOW_RESIZABLE);
    if (not window_result) {
        return tl::unexpected{SDL_GetError()};
    }

    auto renderer_result = ion::renderer::with_default_driver(
            *window_result, SDL_RENDERER_ACCELERATED);
    if (not renderer_result) {
        return tl::unexpected{SDL_GetError()};
    }

    return muncher{*std::move(window_result), *std::move(renderer_result)};
}

muncher::muncher(ion::window && window, ion::renderer && renderer)

      // intialize the window with specified dimensions
    : _window{window}, _renderer{renderer},

      // create a wasd keyboard input axis
      _input(_events, SDLK_d, SDLK_a, SDLK_w, SDLK_s),

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
    while (not ion::input::has_quit()) {
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

void reset_game(SDL_Event const & event)
{
    // do nothing if not the right event or not the right key
    if (event.type != SDL_KEYUP || event.key.keysym.sym != SDLK_r) {
        return;
    }
    get_game().reset();
}
