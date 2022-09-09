// frameworks
#include <ion/ion.hpp>
#include <SDL.h>

// data types
#include <cmath>
#include <cstdint>

// resource management
#include <memory>

// i/o
#include <iostream>

// algorithms
#include <concepts>
#include <ranges>
#include <algorithm>

namespace views = std::views;

// std::lerp two integers
template<std::integral int_t, std::floating_point float_t>
int_t lerp(int_t x, int_t y, float_t t)
{
    return static_cast<int_t>(std::lerp(static_cast<float_t>(x),
                                        static_cast<float_t>(y), t));
}

// std::lerp two SDL_Colors
template<std::floating_point float_t>
SDL_Color lerp(SDL_Color const & a, SDL_Color const & b, float_t t)
{
    return { lerp(a.r, b.r, t), lerp(a.g, b.g, t), lerp(a.b, b.b, t), 0xff };
}

// map each index in a finite range to a linear-interpolated color between a and b
auto color_sequence(std::uint32_t N, SDL_Color const & a, SDL_Color const & b)
{
    return [N, &a, &b](std::uint32_t k) {
        float const t = static_cast<float>(k)/N;
        return lerp(a, b, t);
    };
}

// map each index in a finite range to a rectangle such that the resulting
// sequence of rectangles fills the screen in rotating halves toward the
// bottom-right corner
auto rect_sequence(SDL_Rect & guide)
{
    return [&guide](std::uint32_t k) {
        SDL_Rect const rect = guide;
        // split in half horizontally when k is even
        if (k % 2 == 0) {
            guide.x += guide.w;
            guide.h /= 2;
        }
        // split in half vertically when k is odd
        else {
            guide.y += guide.h;
            guide.w /= 2;
        }
        return rect;
    };
}

// map each index in a finite range to a rect-color pair
auto colored_rect_sequence(std::regular_invocable<std::uint32_t> auto to_rect,
                           std::regular_invocable<std::uint32_t> auto to_color)
{
    return [to_rect, to_color](std::uint32_t k){
        return std::make_pair(to_rect(k), to_color(k));
    };
}

// define how to draw the pattern
void draw(SDL_Renderer * renderer)
{
    // the initial color
    SDL_Color const blue{48, 118, 217, 255};
    // the final color
    SDL_Color const red{219, 0, 66, 255};

    // clear the screen
    SDL_SetRenderDrawColor(renderer, red.r, red.g, red.b, red.a);
    SDL_RenderClear(renderer);

    // a dynamic guide that will determine the dimensions of each rect to draw
    SDL_Rect rect_guide{0, 0, 0, 0};
    SDL_GetRendererOutputSize(renderer, &rect_guide.w, &rect_guide.h);
    rect_guide.w /= 2;

    // draw the fibonacci-like patern
    std::uint32_t const N = 8;
    auto to_colored_rects = colored_rect_sequence(
        rect_sequence(rect_guide),
        color_sequence(N, blue, red));

    auto colored_rects = views::iota(0u, N)
                       | views::transform(to_colored_rects);
    for (auto const & [rect, color] : colored_rects) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_RenderPresent(renderer);
}

int main()
{
    // create the sdl event-handler: quit when sdl's quit event is triggered
    ion::event_system events;
    events.subscribe(SDL_QUIT, &ion::input::quit_on_event);

    // initialize sdl - initialize this before other sdl resources
    auto system_result = ion::system::with_defaults();
    if (not system_result) {
        std::cerr << "Couldn't initialize SDL: "
                  << system_result.error() << "\n";
        return EXIT_FAILURE;
    }

    // create a window, specifying the title and dimensions
    auto window_result = ion::window::at_anywhere("A simple window", 800, 600);
    if (not window_result) {
        std::cerr << "Couldn't create a window: "
                  << window_result.error() << "\n";
        return EXIT_FAILURE;
    }
    auto window = *std::move(window_result);

    auto renderer_result = ion::renderer::with_default_driver(window);
    if (not renderer_result) {
        std::cerr << "Couldn't create a renderer: "
                  << renderer_result.error() << "\n";
        return EXIT_FAILURE;
    }
    auto renderer = *std::move(renderer_result);

    // render the pattern once at the beginning of the program
    draw(renderer);

    // busy loop until the user quits
    while (not ion::input::has_quit()) {
        events.process_queue();
    }
    return EXIT_SUCCESS;
}
