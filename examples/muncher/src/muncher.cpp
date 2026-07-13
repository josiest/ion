#include "muncher.hpp"

#include "entities/player.hpp"
#include "entities/munchable.hpp"

#include "systems/render.hpp"
#include "systems/physics.hpp"
#include "systems/mechanics.hpp"

#include <entt/entity/registry.hpp>
#include <SDL3/SDL_keycode.h>
#include <random>

#include <ion/engine.hpp>
#include <ion/editor.hpp>
#include <ion/time.hpp>

ion::editor * GEditor = nullptr;
muncher & get_game()
{
    static muncher game;
    return game;
}

int main(int argc, char * argv[])
{
    auto editor = ion::editor::initialize();
    if (not editor) { return EXIT_FAILURE; }
    GEditor = editor.get();

    auto & game = get_game();
    game.reset();
    ion::clock clock;
    while (not editor->has_quit())
    {
        const float delta_time = clock.tick();
        ion::sdl_events::poll();
        game.update(delta_time);
    }
    return EXIT_SUCCESS;
}

muncher::muncher()

      // intialize the window with specified dimensions
    : // create a wasd keyboard input axis
      _input(SDLK_D, SDLK_A, SDLK_W, SDLK_S),

      // initialize the random engine with a pseudo-random seed
      _rng{std::random_device{}()}
{
    // check if sdl resources initialized properly
    ion::sdl_events::on_key_up().connect<&reset_game>();
}

void muncher::update(float delta_time)
{
    SDL_Point window_size;
    SDL_GetWindowSize(GEditor->window.get(), &window_size.x, &window_size.y);
    const SDL_FRect bounds{ 0.f, 0.f, static_cast<float>(window_size.x),
                                      static_cast<float>(window_size.y) };
    // create a new munchable entity if it's time to
    if (_munchable_settings.should_munch(delta_time, _rng)) {

        _munchable_settings.create(_entities, _player, bounds,
                                   _player_settings, _rng);
    }
    // physics systems
    systems::accelerate_player(_entities, _player, _input, delta_time);
    systems::move_munchies(_entities, delta_time);

    // mechanics systems
    systems::munch(_entities, _player);

    // get the size of the screen to filter out munchables
    systems::filter_munchables(_entities, window_size.x, window_size.y);

    // render
    systems::render(GEditor->renderer.get(), _entities);
}

void muncher::reset()
{
    // destroy the player if they exist
    if (_entities.valid(_player)) {
        _entities.destroy(_player);
    }
    // then reinstantiate them with default settings
    SDL_Point window_size;
    SDL_GetWindowSize(GEditor->window.get(), &window_size.x, &window_size.y);
    const SDL_FRect bounds{ 0.f, 0.f, static_cast<float>(window_size.x),
                                      static_cast<float>(window_size.y) };
    _player = _player_settings.create(_entities, bounds);
}

void reset_game(SDL_Keycode keycode)
{
    // do nothing if not the right event or not the right key
    if (keycode == SDLK_R) {
        get_game().reset();
    }
}
