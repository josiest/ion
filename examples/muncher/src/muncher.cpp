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

// a class that manages sdl resources
class Muncher {
public:
    using engine_t = std::mt19937;
    // intialize the muncher game object with the specified width and height
    Muncher(size_t width, size_t height)
        : _title{"Muncher"}, _width(width), _height(height), _handler(),
          _input(_handler, SDLK_d, SDLK_a, SDLK_w, SDLK_s), _registry(),
          _rng{std::random_device{}()}, _munchables{_width, _height, _rng}
    {
        // the choice to subscribe the event listeners here is somewhat arbitrary
        _handler.subscribe(SDL_QUIT, &quit);
    }

    // make sure that sdl quits once the game goes out of scope
    ~Muncher() { SDL_Quit(); }

    void run()
    {
        // create the window, specifying title and dimensions
        auto window = ion::basic_window(_title, _width, _height);
        auto player = make_player(_registry, static_cast<int>(_width)/2,
                                             static_cast<int>(_height)/2, 15);

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
                _munchables.make_munchable(_registry);
            }
            accelerate_player(_registry, player, _input, dt);
            move_munchies(_registry, dt);
            devour(_registry, player);
            _munchables.filter(_registry);

            // render
            render(window, _registry);
        }
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
    entt::registry _registry;

    engine_t _rng;
    MunchableFactory _munchables;
};

int main()
{
    // run the muncher game then quit sdl
    Muncher game{800, 600};
    game.run();
}

