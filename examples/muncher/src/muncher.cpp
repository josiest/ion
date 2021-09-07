#include "ion.hpp"
#include <SDL2/SDL.h>
#include <string>

void render(ion::Window & window)
{
    auto renderer = window.sdl_renderer();
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

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
        : _title{"Muncher"}, _width(width), _height(height), _handler()
    {
        // the choice to subscribe the event listeners here is somewhat arbitrary
        _handler.subscribe(SDL_QUIT, &quit);
    }

    // make sure that sdl quits once the game goes out of scope
    ~Muncher() { SDL_Quit(); }

    void run()
    {
        // create the window, specifying title and dimensions
        auto window = ion::unique_basic_window(_title, _width, _height);

        // run the program
        while (!has_quit()) {

            _handler.process_queue();
            render(*window);
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
};

int main()
{
    // run the muncher game then quit sdl
    Muncher game{800, 600};
    game.run();
}

