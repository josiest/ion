// frameworks
#include <ion/ion.hpp>
#include <SDL2/SDL.h>

// data types
#include <cstdint>

// algorithms
#include <cmath>
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
auto color_sequence(SDL_Color const & a, SDL_Color const & b, std::uint32_t N)
{
    return [&a, &b, N](std::uint32_t k) {
        float const t = static_cast<float>(k)/N;
        return lerp(a, b, t);
    };
}

// map each index in a finite range to a rectangle such that the resulting
// sequence of rectangles fills the screen in rotating halves toward the
// bottom-right corner
auto rect_sequence(SDL_Rect & guide)
{
    guide.w /= 2;
    return [&guide](std::uint32_t k) {
        // copy the guide's current state to return later
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
auto colored_rect_sequence(std::regular_invocable<std::uint32_t> auto && to_rect,
                           std::regular_invocable<std::uint32_t> auto && to_color)
{
    return [&to_rect, &to_color](std::uint32_t k){
        return std::make_pair(to_rect(k), to_color(k));
    };
}

// Tell SDL how to draw the fibonacci-like pattern
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
    SDL_Rect guide_rect{ 0, 0, 0, 0 };
    SDL_GetRendererOutputSize(renderer, &guide_rect.w, &guide_rect.h);

    // draw the fibonacci-like patern
    std::uint32_t const N = 8;
    auto to_colored_rect = colored_rect_sequence(
            rect_sequence(guide_rect),
            color_sequence(blue, red, N));

    auto colored_rects = views::iota(0u, N)
                       | views::transform(to_colored_rect);

    for (auto const & [rect, color] : colored_rects) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_RenderPresent(renderer);
}

int main(int argc, char * argv[])
{
    // create the sdl event-handler: quit when sdl's quit event is triggered
    ion::event_system events;
    events.subscribe(SDL_QUIT, &ion::input::quit_on_event);

    // initialize sdl - initialize this before other sdl resources
    ion::sdl_context sdl;

    // create a window, specifying the title and dimensions
    auto window = ion::hardware_renderer::basic_window("A simple window", 800, 600);
    draw(window); // render once at the beginning of the program

    // busy loop until the user quits
    while (not ion::input::has_quit()) {
        events.process_queue();
    }
    return EXIT_SUCCESS;
}
