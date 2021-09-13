#include <ion/ion.hpp>
#include <SDL2/SDL.h>

void render(ion::render_window & window);

// creating a function to limit the scope of certain variables
// local variables will be forced to destruct before quitting SDL
void run_simple_program()
{
    // create the sdl event-handler
    ion::event_system events;
    events.subscribe(SDL_QUIT, &ion::input::quit_on_event);

    // specify the program's title and dimensions
    auto window = ion::basic_window("A simple window", 800, 600);
    render(window);

    // run the program
    while (!ion::input::has_quit()) {
        events.process_queue();
    }
}

// draw a fibonacci-like pattern
void render(ion::render_window & window)
{
    // the initial color
    int r0 = 48, g0 = 118, b0 = 217;
    // the final color
    int r1 = 219, g1 = 0, b1 = 66;

    // clear the screen
    auto renderer = window.sdl_renderer();
    SDL_SetRenderDrawColor(renderer, r1, g1, b1, 0xff);
    SDL_RenderClear(renderer);

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
        SDL_SetRenderDrawColor(renderer, r, g, b, 0xff);
        SDL_RenderFillRect(renderer, &rect);

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
    SDL_RenderPresent(renderer);
}

int main()
{
    run_simple_program();
    // finally, quit SDL
    SDL_Quit();
}
