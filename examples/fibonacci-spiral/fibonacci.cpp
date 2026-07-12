// frameworks
#include <ion/engine.hpp>
#include <ion/editor.hpp>
#include <ion/math.hpp>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>

// algorithms
#include <cmath>
#include <ranges>
#include <numbers>

namespace views = std::views;

class spiral_data
{
public:
    SDL_Color initial_color{ 48, 118, 217, 255 };
    SDL_Color final_color{ 219, 0, 66, 255 };
    std::uint8_t num_frames = 8u;
};
// namespace ion {
// template<>
// auto reflect<spiral_data>()
// {
//     using namespace entt::literals;
//     ion::reflect<SDL_Color>();
//     return entt::meta_factory<spiral_data>()
//         .type("spiral_data"_hs)
//         .data<&spiral_data::initial_color>("initial-color"_hs)
//         .data<&spiral_data::final_color>("final-color"_hs)
//         .data<&spiral_data::num_frames>("num-frames"_hs);
// }
// }

class fibonacci_spiral
{
public:
    spiral_data settings;

    fibonacci_spiral() = delete;
    explicit fibonacci_spiral(const spiral_data& spiral)
        : settings{ spiral }
    {
    }

    void draw_to(SDL_Renderer * renderer)
    {
        SDL_Point output_size;
        SDL_GetRenderOutputSize(renderer, &output_size.x, &output_size.y);
        const SDL_FRect render_frame{ 0.f, 0.f, static_cast<float>(output_size.x),
                                                static_cast<float>(output_size.y) };
        draw_to(renderer, render_frame);
    }

    void draw_to(SDL_Renderer * renderer, const SDL_FRect & render_frame)
    {
        SDL_SetRenderDrawColor(renderer, settings.initial_color.r, settings.initial_color.g,
                                         settings.initial_color.b, settings.initial_color.a);
        SDL_RenderFillRect(renderer, &render_frame);

        guide = render_frame;

        // MinGW compiler can't seem to deduce the return type of view_colored_rects(), but CLion can?
        // maybe undefined behavior?? https://timsong-cpp.github.io/cppwp/n4140/dcl.spec.auto
        // for (const auto [rect, color] : view_colored_rects()) {
        for (std::uint8_t k = 0; k < settings.num_frames; ++k)
        {
            const auto [rect, color] = next_colored_rect(k);
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &rect);
        }
        guide = render_frame;
    }

private:
    static constexpr float DEFAULT_WIDTH = 640.f;
    static constexpr float DEFAULT_HEIGHT = 480.f;

    SDL_FRect guide{ 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT };

    std::ranges::view auto view_colored_rects()
    {
        constexpr std::uint8_t zero = 0u;
        return std::views::iota(zero, settings.num_frames)
             | views::transform([this](std::uint8_t k) { return next_colored_rect(k); });
    }

    std::pair<SDL_FRect, SDL_Color> next_colored_rect(std::uint8_t k)
    {
        const float t = static_cast<float>(k)/static_cast<float>(settings.num_frames);
        return std::make_pair(next_subframe(k), ion::lerp(settings.initial_color, settings.final_color, t));
    }

    SDL_FRect next_subframe(std::uint32_t k)
    {
        using namespace std::numbers;
        const SDL_FRect subframe = guide;
        const SDL_FPoint next_frame{  std::round(guide.w/phi_v<float>),
                                      std::round(guide.h/phi_v<float>) };
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
    const auto editor = ion::editor::initialize();
    if (not editor) { return EXIT_FAILURE; }

    // load the spiral settings and draw it to the whole window
    // ion::asset_loader asset_loader{};
    // const auto spiral_settings = asset_loader.from_config<spiral_data>(settings["spiral"]);
    constexpr spiral_data spiral_settings;
    auto spiral = fibonacci_spiral{ spiral_settings };

    spiral.draw_to(editor->renderer.get());
    SDL_RenderPresent(editor->renderer.get());
    while (not editor->has_quit())
    {
        sdl_events::poll();
    }
    return EXIT_SUCCESS;
}
