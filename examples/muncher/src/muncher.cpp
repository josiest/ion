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
        std::cerr << "Coudln't initialize muncher: "
                  << game_result.error() << "\n";
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
    return EXIT_SUCCESS;
}

tl::expected<muncher, std::string>
muncher::with_dimensions(std::uint32_t width, std::uint32_t height)
{
    auto system_result = ion::system::with_defaults();
    if (not system_result) {
        return tl::unexpected{system_result.error()};
    }
    return muncher(*std::move(system_result), width, height);
}

muncher::muncher(ion::system && sys,
                 std::uint32_t width, std::uint32_t height)

    : _system{std::move(sys)},

      // intialize the window with specified dimensions
      _window{ion::hardware_renderer::basic_window(
            "Muncher", width, height, SDL_WINDOW_RESIZABLE)},

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
    if (not _window) {
        set_error(_window.get_error()); return;
    }

    // quit when the user exits the window
    _events.subscribe(SDL_QUIT, &ion::input::quit_on_event);

    // reset the game when the user presses the reset key
    _events.subscribe_functor(SDL_KEYUP, [this](SDL_Event const & event) {
        if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_r) {
            reset();
        }
    });
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
