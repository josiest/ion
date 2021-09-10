#include "ion/ion.hpp"
#include "entities/player.hpp"
#include "systems/render.hpp"

#include <SDL2/SDL.h>
#include <string>
#include <iostream>

// resource interface for quitting the program
bool HAS_QUIT = false;

// determine if the user has quit the game
bool has_quit() { return HAS_QUIT; }

// an event-callback to set the quit-flag to true
void quit(SDL_Event const &) { HAS_QUIT = true; }

// a class that manages sdl resources
class Muncher {
public:
    // intialize the muncher game object with the specified width and height
    Muncher(size_t width, size_t height)
        : _title{"Muncher"}, _width(width), _height(height), _handler(),
          _input(_handler, SDLK_d, SDLK_a, SDLK_w, SDLK_s), _registry()
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
        make_player(_registry, static_cast<int>(_width)/2,
                               static_cast<int>(_height)/2, 15);

        // run the program
        while (!has_quit()) {

            _handler.process_queue();
            render(window, _registry);

            float const x = _input.x();
            float const y = _input.y();
            if (x != 0.f) {
                std::cout << "x: " << x << " ";
            }
            if (y != 0.f) {
                std::cout << "y: " << y;
            }
            if (y != 0.f || x != 0.f) {
                std::cout << std::endl;
            }
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
};

int main()
{
    // run the muncher game then quit sdl
    Muncher game{800, 600};
    game.run();
}

