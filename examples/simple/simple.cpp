#include <ion/ion.hpp>
#include <SDL.h>

#include <cmath>
#include <cstdint>

#include <iostream>

SDL_Color lerp(SDL_Color const & a, SDL_Color const & b, float t)
{
    auto intlerp = [](std::uint8_t x, std::uint8_t y, float t) {
        return static_cast<std::uint8_t>(std::lerp(x, y, t));
    };
    return SDL_Color{intlerp(a.r, b.r, t), intlerp(a.g, b.g, t),
                     intlerp(a.b, b.b, t), 0xff};
}

void render(ion::window & window, ion::renderer & renderer)
{
    // the initial color
    SDL_Color blue{48, 118, 217, 255};
    // the final color
    SDL_Color red{219, 0, 66, 255};

    // clear the screen
    SDL_SetRenderDrawColor(renderer, red.r, red.g, red.b, red.a);
    SDL_RenderClear(renderer);

    // the dimensions of the rect to draw
    SDL_Rect rect{0, 0, 0, 0};
    SDL_GetWindowSize(window, &rect.w, &rect.h);
    rect.w /= 2;

    // draw the fibonacci-like patern
    int n = 8;
    for (int k = 0; k < n; k++) {

        // calculate the intermediate color
        float const t = static_cast<float>(k)/n;
        auto const c = lerp(blue, red, t);

        // draw the rect
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
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
    // create the sdl event-handler: quit when sdl's quit event is triggered
    ion::event_system events;
    events.subscribe(SDL_QUIT, &ion::input::quit_on_event);

    // initialize sdl - initialize this before other sdl resources
    ion::sdl_context sdl;

    // create a window, specifying the title and dimensions
    auto window_result = ion::window::at_anywhere(
        "A simple window", 800, 600);
    if (not window_result) {
        std::cerr << "Couldn't load window: "
                  << window_result.error() << "\n";
        return EXIT_FAILURE;
    }
    auto window = *std::move(window_result);

    auto renderer_result = ion::renderer::with_default_driver(
        window, SDL_RENDERER_ACCELERATED);
    if (not renderer_result) {
        std::cerr << "Couldn't create renderer: "
                  << renderer_result.error() << "\n";
        return EXIT_FAILURE;
    }
    auto renderer = *std::move(renderer_result);

    render(window, renderer); // render once at the beginning of the program

    // busy loop until the user quits
    while (not ion::input::has_quit()) {
        events.process_queue();
    }
    return EXIT_SUCCESS;
}
