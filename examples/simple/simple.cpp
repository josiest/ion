#include "ion.hpp"
#include "SDL2/SDL.h"

// draw a fibonacci-like pattern
void render(ion::Window & window)
{
    // the initial color
    int r0 = 48, g0 = 118, b0 = 217;
    // the final color
    int r1 = 219, g1 = 0, b1 = 66;

    // clear the screen
    SDL_SetRenderDrawColor(window.sdl_renderer(), r1, g1, b1, 0xff);
    SDL_RenderClear(window.sdl_renderer());

    // the dimensions of the rect to draw
    SDL_Rect rect{0, 0, 0, 0};
    SDL_GetWindowSize(window.sdl_window(), &rect.w, &rect.h);
    rect.w /= 2;

    // draw the fibonacci-like patern
    int n = 8;
    for (int k = 0; k < n; k++) {

        // calculate the intermediate color
        float t = (float)k/n;
        int r = (int)((1-t)*r0 + t*r1);
        int g = (int)((1-t)*g0 + t*g1);
        int b = (int)((1-t)*b0 + t*b1);

        // draw the rect
        SDL_SetRenderDrawColor(window.sdl_renderer(), r, g, b, 0xff);
        SDL_RenderFillRect(window.sdl_renderer(), &rect);

        // split in half horizontally when k is even
        if (k % 2 == 0) {
            rect.x += rect.w;
            rect.h /= 2;
        }
        // split in half vertically when k is odd
        else {
            rect.y += rect.h;
            rect.w /= 2;
        }
    }
    SDL_RenderPresent(window.sdl_renderer());
}

// since this framework relies on function pointers in order to handle events,
// we can't pass capturing lambdas to the event handlers!
// thus (at least to my knowledge), we're limited to using a global resource
bool HAS_QUIT = false;
void quit(SDL_Event const &) { HAS_QUIT = true; }
// NOTE: in general, it might be a better idea to create a resources class with
// an API for managing its resources. But since this example only needs to know
// when the user quit, we'll just keep things as simple as possible

// creating a function to limit the scope of certain variables
// local variables will be forced to destruct before quitting SDL
void run_simple_program()
{
    // create the sdl event-handler
    ion::EventHandler handler;
    handler.subscribe(SDL_QUIT, &quit);

    // specify the program's title and dimensions
    auto window = ion::unique_basic_window("A simple window", 800, 600);
    render(*window);

    // run the program
    while (!HAS_QUIT) {
        handler.process_queue();
    }
}

int main(int argc, char * argv[])
{
    run_simple_program();
    // finally, quit SDL
    SDL_Quit();

    return 0;
}
