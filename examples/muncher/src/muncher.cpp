#include "entities/player.hpp"
#include "entities/munchable.hpp"

#include "systems/render.hpp"
#include "systems/physics.hpp"

#include <ion/ion.hpp>
#include <entt/entity/registry.hpp>
#include <SDL2/SDL.h>

#include <string>
#include <random>

void reset_game(SDL_Event const & event);

// a class that manages sdl resources
class muncher {
public:
    using engine_t = std::mt19937;

    muncher(size_t width, size_t height)

          // intialize the muncher game object with the specified width and height
        : _title{"Muncher"}, _width(width), _height(height),

          // create a wasd keyboard input axis
          _input(_events, SDLK_d, SDLK_a, SDLK_w, SDLK_s),

          // initialize the random engine to a random seed
          _rng{std::random_device{}()}, 

          // create the player entity and the munchable factory
          _player(_make_player()), _munchables{_width, _height, _rng}
    {
        _events.subscribe(SDL_QUIT, &ion::input::quit_on_event);
        _events.subscribe(SDL_KEYUP, &reset_game);
    }

    // make sure that sdl quits once the game goes out of scope
    ~muncher() { SDL_Quit(); }

    void run()
    {
        // declare the sdl context before any other sdl resources
        ion::sdl_context sdl;

        // create the window, specifying title and dimensions
        auto window = ion::render_window(_title, _width, _height);

        // timer for physics
        ion::clock clock;

        // decider for making a munchable
        std::binomial_distribution<bool> is_munch_time(1, .01);

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
            render(window, _entities);
        }
    }
    void reset()
    {
        if (_entities.valid(_player)) {
            _entities.destroy(_player);
        }
        _player = _make_player();
    }
private:
    std::string const _title;
    size_t _width;
    size_t _height;

    ion::event_system _events;
    ion::input::keyboard_axis _input;

    engine_t _rng;
    entt::registry _entities;

    entt::entity _player;
    munchable_factory _munchables;

    entt::entity _make_player()
    {
        return make_player(_entities, static_cast<int>(_width)/2,
                                      static_cast<int>(_height)/2, 15);
    }
};

muncher GAME{800, 600};
void reset_game(SDL_Event const & event)
{
    // do nothing if not the right event or not the right key
    if (event.type != SDL_KEYUP || event.key.keysym.sym != SDLK_r) {
        return;
    }
    GAME.reset();
}

int main()
{
    // run the muncher game then quit sdl
    GAME.run();
}

