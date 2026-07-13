// frameworks
#include <ion/engine.hpp>
#include <ion/editor.hpp>
#include <ion/math.hpp>
#include <ion/serialization.hpp>
#include <ion/mylar.hpp>

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

namespace ion
{
template<>
auto reflect<spiral_data>()
{
    using namespace entt::literals;
    YAML::convert<SDL_Color>::reflect();
    return entt::meta_factory<spiral_data>()
        .type("spiral_data"_hs)
        .data<&spiral_data::initial_color>("initial-color"_hs)
        .data<&spiral_data::final_color>("final-color"_hs)
        .data<&spiral_data::num_frames>("num-frames"_hs);
}
}

/** Transform a guide rect into the next subframe of the fibonacci spiral
 *
 * \param guide changed into the next fibonacci spiral frame
 * \param k     used to determine which edge to transform
 *
 * \return the current value of the guide rect unchanged
 */
constexpr SDL_FRect next_subframe(SDL_FRect& guide, std::uint32_t k)
{
    using namespace std::numbers;
    const SDL_FRect rect = guide;
    const SDL_FPoint frame{ std::round(guide.w/phi_v<float>),
                            std::round(guide.h/phi_v<float>) };
    switch (k % 4) {
    case 0:
        // slide the left edge inward by the golden ratio
        guide.x += frame.x; guide.w -= frame.x;
        break;
    case 1:
        // slide the top edge down by the golden ratio
        guide.y += frame.y; guide.h -= frame.y;
        break;
    case 2:
        // slide the right edge inward by the golden ratio
        guide.w -= frame.x;
        break;
    case 3:
        // slide the bottom edge upward by the golden ratio
        guide.h -= frame.y;
        break;
    default: break;
    }
    return rect;
}

/** Draw a colored rect to a renderer */
inline void fill_rect(SDL_Renderer* renderer, const SDL_FRect& rect, const SDL_Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

/** Generate the fibonacci spiral sequence
 *
 * \param spiral The settings that describe how to draw the spiral
 * \param guide The frame to generate te spiral in
 * \return a function that generates the fibonacci spiral sequence
 *
 * As a side effect, the guide will be changed with every call
 * of the generator function
 */
inline auto generate_sequence(const spiral_data& spiral, SDL_FRect& guide)
{
    return [&](std::uint32_t frame_index)
    {
        const float t = static_cast<float>(frame_index)/spiral.num_frames;
        const auto color = ion::lerp(spiral.initial_color, spiral.final_color, t);
        return std::make_pair(next_subframe(guide, frame_index), color);
    };
}

/** Draw the fibonacci spiral to the renderer */
inline void draw_spiral(SDL_Renderer* renderer, const spiral_data& spiral)
{
    namespace ranges = std::ranges;
    namespace views = std::ranges::views;

    // the pattern will be drawn to the entire screen
    SDL_Point output_size;
    SDL_GetRenderOutputSize(renderer, &output_size.x, &output_size.y);
    SDL_FRect render_frame{ 0.f, 0.f, static_cast<float>(output_size.x),
                                      static_cast<float>(output_size.y) };

    // render each subframe with the next color on the gradient
    auto colored_rect_sequence = views::iota(0u, spiral.num_frames)
                               | views::transform(generate_sequence(spiral, render_frame));
    for (const auto& [rect, color] : colored_rect_sequence)
    {
        fill_rect(renderer, rect, color);
    }
}

int main(int argc, char * argv[])
{
    const auto editor = ion::editor::initialize();
    if (not editor) { return EXIT_FAILURE; }

    // load the spiral settings and draw it to the whole window
    const auto spiral = ion::read_yaml<spiral_data>(ion::editor_settings::load_setting("spiral"));

    draw_spiral(editor->renderer.get(), spiral);
    SDL_RenderPresent(editor->renderer.get());
    while (not editor->has_quit())
    {
        ion::sdl_events::poll();
    }
    return EXIT_SUCCESS;
}
