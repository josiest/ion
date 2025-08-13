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

constexpr int rounded_divide(int numerator, int denominator)
{
    return (numerator + denominator/2)/denominator;
}

template<std::integral Integer>
constexpr Integer intlerp(int a, int b, int p, int q)
{
    // convert
    return static_cast<Integer>(a + rounded_divide(p*(b-a), q));
}

constexpr SDL_Color lerp(SDL_Color const & a, SDL_Color const & b, std::uint8_t p, std::uint8_t q)
{
    return { intlerp<std::uint8_t>(a.r, b.r, p, q), intlerp<std::uint8_t>(a.g, b.g, p, q),
             intlerp<std::uint8_t>(a.b, b.b, p, q), intlerp<std::uint8_t>(a.a, b.a, p, q) };
}

/** Divide an integer by a floating point number and round to nearest */
template<std::integral Integer>
constexpr Integer divide_by_phi(Integer numerator)
{
    // arbitrary rational approximation of phi: P/Q
    constexpr Integer P = 233, Q = 144;
    return rounded_divide(numerator*Q, P);
}

class fibonacci_spiral
{
public:
    SDL_Color initial_color{ 48, 118, 217, 255 };
    SDL_Color final_color{ 219, 0, 66, 255 };
    std::uint8_t num_frames = 8;

    void draw_to(SDL_Renderer * renderer) const
    {
        SDL_Rect render_frame{ 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT };
        SDL_GetRendererOutputSize(renderer, &render_frame.w, &render_frame.h);
        draw_to(renderer, render_frame);
    }

    void draw_to(SDL_Renderer * renderer, const SDL_Rect & render_frame) const
    {
        SDL_SetRenderDrawColor(renderer, initial_color.r, initial_color.g, initial_color.b, initial_color.a);
        SDL_RenderFillRect(renderer, &render_frame);

        guide = render_frame;

        // MinGW compiler can't seem to deduce the return type of view_colored_rects(), but CLion can?
        // maybe undefined behavior?? https://timsong-cpp.github.io/cppwp/n4140/dcl.spec.auto
        // for (const auto [rect, color] : view_colored_rects()) {
        for (std::uint8_t k = 0; k < num_frames; ++k)
        {
            const auto [rect, color] = next_colored_rect(k);
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &rect);
        }
        guide = render_frame;
    }

private:
    static constexpr int DEFAULT_WIDTH = 640;
    static constexpr int DEFAULT_HEIGHT = 480;

    mutable SDL_Rect guide{ 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT };

    std::ranges::view auto view_colored_rects() const
    {
        constexpr std::uint8_t zero = 0u;
        return std::views::iota(zero, num_frames)
             | views::transform([this](std::uint8_t k) { return next_colored_rect(k); });
    }

    std::pair<SDL_Rect, SDL_Color> next_colored_rect(std::uint8_t k) const
    {
        return std::make_pair(next_subframe(k), lerp(initial_color, final_color, k, num_frames));
    }

    SDL_Rect next_subframe(std::uint32_t k) const
    {
        using namespace std::numbers;
        const SDL_Rect subframe = guide;
        const SDL_Point next_frame{ divide_by_phi(guide.w), divide_by_phi(guide.h) };
        switch (k % 4) {
            case 0:
                // slide the left edge inward by the golden ratio
                guide.x += next_frame.x; guide.w -= next_frame.x;
                break;
            case 1:
                // slide the top edge down by the golden ratio
                guide.y += next_frame.y; guide.h -= next_frame.y;
                break;
            case 2:
                // slide the right edge inward by the golden ratio
                guide.w -= next_frame.x;
                break;
            case 3:
                // slide the bottom edge upward by the golden ratio
                guide.h -= next_frame.y;
                break;
            default: break;
        }
        return subframe;
    }
};

int main(int argc, char * argv[])
{
    // create the sdl event-handler: quit when sdl's quit event is triggered
    ion::event_system events;
    events.subscribe(SDL_QUIT, &ion::input::quit_on_event);

    // initialize sdl and create a window
    ion::sdl_context sdl;
    auto window = ion::hardware_renderer::basic_window("Fibonacci Spiral", 800, 600);

    // load the spiral settings and draw it to the whole window
    constexpr fibonacci_spiral spiral;
    spiral.draw_to(window);
    SDL_RenderPresent(window);

    // busy loop until the user quits
    while (not ion::input::has_quit()) {
        events.process_queue();
    }
    return EXIT_SUCCESS;
}
