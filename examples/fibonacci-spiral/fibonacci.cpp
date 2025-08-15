// frameworks
#include <ion/ion.hpp>
#include <ion/mylar.hpp>
#include <SDL2/SDL.h>
#include <entt/signal/sigh.hpp>
#include <entt/meta/factory.hpp>

// serialization
#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>
#include <ion/serialization/yaml.hpp>

// data types
#include <cstdint>
#include <filesystem>

// algorithms
#include <cmath>
#include <concepts>
#include <ranges>

namespace views = std::views;
const std::filesystem::path RESOURCE_DIR = "resources";

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

class spiral_data
{
public:
    SDL_Color initial_color{ 48, 118, 217, 255 };
    SDL_Color final_color{ 219, 0, 66, 255 };
    std::uint8_t num_frames;
};

namespace ion {
template<>
auto reflect<spiral_data>()
{
    using namespace entt::literals;
    ion::reflect<SDL_Color>();
    return entt::meta_factory<spiral_data>()
        .type("spiral_data"_hs)
        .data<&spiral_data::initial_color>("initial-color"_hs)
        .data<&spiral_data::final_color>("final-color"_hs)
        .data<&spiral_data::num_frames>("num-frames"_hs);
}
}

class fibonacci_spiral
{
public:
    spiral_data spiral;

    fibonacci_spiral() = delete;
    fibonacci_spiral(const spiral_data& spiral)
        : spiral{ spiral }
    {
    }

    static spiral_data from_config(const YAML::Node & config)
    {
        spiral_data spiral;
        try
        {
            if (config.IsScalar())
            {
                auto filepath = RESOURCE_DIR/config.as<std::string>();
                filepath.replace_extension(".yaml");
                spiral = from_config(YAML::LoadFile(filepath.string()));
            }
            else
            {
                if (const auto& initial_color_config = config["initial-color"])
                {
                    spiral.initial_color = initial_color_config.as<SDL_Color>();
                }
                if (const auto& final_color_config = config["final-color"])
                {
                    spiral.final_color = final_color_config.as<SDL_Color>();
                }
                if (const auto& num_frames_config = config["num-frames"])
                {
                    spiral.num_frames = num_frames_config.as<std::uint8_t>();
                }
            }
        }
        catch (const YAML::Exception & error)
        {
            SDL_Log("Unable to read spiral from config: %s\n", error.what());
        }
        return spiral;
    }

    void draw_to(SDL_Renderer * renderer)
    {
        SDL_Rect render_frame{ 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT };
        SDL_GetRendererOutputSize(renderer, &render_frame.w, &render_frame.h);
        draw_to(renderer, render_frame);
    }

    void draw_to(SDL_Renderer * renderer, const SDL_Rect & render_frame)
    {
        SDL_SetRenderDrawColor(renderer, spiral.initial_color.r, spiral.initial_color.g,
                                         spiral.initial_color.b, spiral.initial_color.a);
        SDL_RenderFillRect(renderer, &render_frame);

        guide = render_frame;

        // MinGW compiler can't seem to deduce the return type of view_colored_rects(), but CLion can?
        // maybe undefined behavior?? https://timsong-cpp.github.io/cppwp/n4140/dcl.spec.auto
        // for (const auto [rect, color] : view_colored_rects()) {
        for (std::uint8_t k = 0; k < spiral.num_frames; ++k)
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

    SDL_Rect guide{ 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT };

    std::ranges::view auto view_colored_rects()
    {
        constexpr std::uint8_t zero = 0u;
        return std::views::iota(zero, spiral.num_frames)
             | views::transform([this](std::uint8_t k) { return next_colored_rect(k); });
    }

    std::pair<SDL_Rect, SDL_Color> next_colored_rect(std::uint8_t k)
    {
        return std::make_pair(next_subframe(k), lerp(spiral.initial_color, spiral.final_color, k, spiral.num_frames));
    }

    SDL_Rect next_subframe(std::uint32_t k)
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

class event_sink
{
public:
    void poll()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit.publish();
                break;
            }
        }
    }

    inline auto on_quit() { return entt::sink{ quit }; }
protected:
    entt::sigh<void()> quit;
};

int main(int argc, char * argv[])
{
    namespace cereal = ion::cereal;
    // create the sdl event-handler: quit when sdl's quit event is triggered
    event_sink events;
    events.on_quit().connect<&ion::input::quit>();

    YAML::Node settings;
    try
    {
        settings = YAML::LoadFile("settings.yaml");
    }
    catch (YAML::Exception & error)
    {
        SDL_Log("failed to load program settings, using defaults: %s\n", error.what());
    }

    // initialize sdl and create a window
    ion::sdl_context sdl;
    auto window = cereal::read_renderer_from_config(settings["window"]);
    // auto window = cereal::load_yaml<ion::hardware_renderer>(settings["window"]);

    // load the spiral settings and draw it to the whole window
    auto spiral = fibonacci_spiral{ fibonacci_spiral::from_config(settings["spiral"]) };
    // auto spiral = fibonacci_spiral{ konbu::read<spiral_data>(settings["spiral"]) };
    spiral.draw_to(window);
    SDL_RenderPresent(window);

    // busy loop until the user quits
    while (not ion::input::has_quit()) {
        events.poll();
    }
    return EXIT_SUCCESS;
}
