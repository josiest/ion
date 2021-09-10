#include "ion/ion.hpp"

#include "entities/player.hpp"
#include "entities/munchable.hpp"

#include "systems/render.hpp"
#include "systems/physics.hpp"

#include <SDL2/SDL.h>

#include <string>
#include <random>

// resource interface for quitting the program
bool HAS_QUIT = false;

// determine if the user has quit the game
bool has_quit() { return HAS_QUIT; }

// an event-callback to set the quit-flag to true
void quit(SDL_Event const &) { HAS_QUIT = true; }

void reset_game(SDL_Event const & event);

// a class that manages sdl resources
class Muncher {
public:
    using engine_t = std::mt19937;
    // intialize the muncher game object with the specified width and height
    Muncher(size_t width, size_t height)
        : _title{"Muncher"}, _width(width), _height(height),
          _handler(), _input(_handler, SDLK_d, SDLK_a, SDLK_w, SDLK_s),
          _rng{std::random_device{}()}, 
          _registry(),
          _player(_make_player()), _munchables{_width, _height, _rng}
    {
        // the choice to subscribe the event listeners here is somewhat arbitrary
        _handler.subscribe(SDL_QUIT, &quit);
        _handler.subscribe(SDL_KEYUP, &reset_game);
    }

    // make sure that sdl quits once the game goes out of scope
    ~Muncher() { SDL_Quit(); }

    void run()
    {
        // create the window, specifying title and dimensions
        auto window = ion::basic_window(_title, _width, _height);

        // timer for physics
        uint32_t prev_time = SDL_GetTicks();

        // decider for making a munchable
        std::binomial_distribution<bool> munch_time(1, .01);

        // run the program
        while (!has_quit()) {
            // update physics timer
            uint32_t const current_time = SDL_GetTicks();
            float const dt = static_cast<float>(current_time-prev_time)/1000.f;
            prev_time = current_time;

            // handle events
            _handler.process_queue();

            // apply systems
            if (munch_time(_rng)) {
                _munchables.make_munchable(_registry, _player);
            }
            accelerate_player(_registry, _player, _input, dt);
            move_munchies(_registry, dt);
            devour(_registry, _player);
            _munchables.filter(_registry);

            // render
            render(window, _registry);
        }
    }

    entt::entity _make_player()
    {
        return make_player(_registry, static_cast<int>(_width)/2,
                                      static_cast<int>(_height)/2, 15);
    }
    void reset()
    {
        if (_registry.valid(_player)) {
            _registry.destroy(_player);
        }
        _player = _make_player();
    }
private:
    std::string const _title;
    size_t _width;
    size_t _height;

    // as hinted at before, this could have been declared as a local variable
    // in run, but it was somewhat arbitrarily chosen to be a member variable
    ion::EventHandler _handler;
    // that being said, it's a bad idea to declare SDL-wrapper objects as member
    // variables of a game/app-manager class like this (notice: window is a
    // local variable of run). this is because we want them to be destroyed
    // _before_ `SDL_Quit` is called.
    //
    // This _could_ be achieved by explicitly calling `window.reset()` before
    // `SDL_Quit()`, but having this done implicitly via scoping rules is
    // arguably more elegant
    ion::input::KeyboardAxis2D _input;

    engine_t _rng;
    entt::registry _registry;

    entt::entity _player;
    MunchableFactory _munchables;
};

Muncher GAME{800, 600};
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

