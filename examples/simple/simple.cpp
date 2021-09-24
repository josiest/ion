#include <ion/ion.hpp>
#include <SDL.h>
#include <cmath>

#include <iostream>

void render(ion::renderable_window auto & window);

int main()
{
    // initialize sdl - initialize this before other sdl resources
    ion::sdl_context sdl;

    // make sure sdl properly initialized
    if (not sdl) {
        std::cout << "Couldn't initialize SDL: " << sdl.error() << std::endl;
        return 1;
    }

    // create a window, specifying the title and dimensions
    ion::render_window window{"A simple window", 800, 600};

    // make sure the window properly initialized
    if (not window) {
        std::cout << "Couldn't create a window: "
                  << window.error() << std::endl;
        return 1;
    }

    // create the sdl event-handler: quit when sdl's quit event is triggered
    ion::event_system events;
    events.subscribe(SDL_QUIT, &ion::input::quit_on_event);

    // render once at the beginning of the program
    render(window);

    // busy loop until the user quits
    while (!ion::input::has_quit()) {
        events.process_queue();
    }
}

// draw a fibonacci-like pattern
void render(ion::renderable_window auto & window)
{
    // the initial color
    int r0 = 48, g0 = 118, b0 = 217;
    // the final color
    int r1 = 219, g1 = 0, b1 = 66;

    // clear the screen
    SDL_SetRenderDrawColor(window, r1, g1, b1, 0xff);
    SDL_RenderClear(window);

    // the dimensions of the rect to draw
    SDL_Rect rect{0, 0, 0, 0};
    SDL_GetWindowSize(window, &rect.w, &rect.h);
    rect.w /= 2;

    // draw the fibonacci-like patern
    int n = 8;
    auto intlerp = [](int a, int b, float t) {
        return static_cast<int>(std::lerp(a, b, t));
    };
    for (int k = 0; k < n; k++) {

        // calculate the intermediate color
        float const t = static_cast<float>(k)/n;
        int const r = intlerp(r0, r1, t);
        int const g = intlerp(g0, g1, t);
        int const b = intlerp(b0, b1, t);

        // draw the rect
        SDL_SetRenderDrawColor(window, r, g, b, 0xff);
        SDL_RenderFillRect(window, &rect);

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
    SDL_RenderPresent(window);
}
