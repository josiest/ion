#include "ion.hpp"
#include <SDL2/SDL.h>

void render(ion::Window & window)
{
    auto renderer = window.sdl_renderer();
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

// resource interface for quitting the program
bool HAS_QUIT = false;
bool has_quit() { return HAS_QUIT; }
void quit(SDL_Event const &) { HAS_QUIT = true; }

void muncher()
{
    // create the sdl-event handler
    ion::EventHandler handler;
    handler.subscribe(SDL_QUIT, &quit);

    // create the window, specifying title and dimensions
    auto window = ion::unique_basic_window("Muncher", 800, 600);

    // run the program
    while (!has_quit()) {

        handler.process_queue();
        render(*window);
    }
}

int main()
{
    // run the muncher game then quit sdl
    muncher();
    SDL_Quit();
}

