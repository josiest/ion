#pragma once

#include <memory>
#include <string>
#include <optional>
#include <cstdint>
#include <SDL3/SDL_init.h>
#include "ion/containers.hpp"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;

namespace ion
{
namespace internal
{
class sdl_lifetime_helper
{
public:
    sdl_lifetime_helper() = delete;
    explicit sdl_lifetime_helper(std::uint32_t subsystem_flags);
};

class sdl_system_DEPRECATED
{
public:
    sdl_system_DEPRECATED() = delete;
    explicit sdl_system_DEPRECATED(std::uint32_t init_flags);

    bool was_init() const { return was_init_; }
private:
    bool was_init_ = false;
};

struct sdl_deleter
{
    void operator()(const sdl_lifetime_helper * ctx) const;
    void operator()(const sdl_system_DEPRECATED * sdl) const;
    void operator()(SDL_Window * window) const;
    void operator()(SDL_Renderer * renderer) const;
    void operator()(SDL_Surface * surface) const;
};
}

using sdl_context = std::unique_ptr<internal::sdl_lifetime_helper, internal::sdl_deleter>;
using sdl_system_DEPRECATED = std::unique_ptr<internal::sdl_system_DEPRECATED, internal::sdl_deleter>;
using sdl_window = std::unique_ptr<SDL_Window, internal::sdl_deleter>;
using sdl_renderer = std::unique_ptr<SDL_Renderer, internal::sdl_deleter>;
using sdl_surface = std::unique_ptr<SDL_Surface, internal::sdl_deleter>;

sdl_context init_sdl(std::uint32_t subsystem_flags);
sdl_system_DEPRECATED init_sdl_DEPRECATED(std::uint32_t init_flags);
sdl_window create_window(std::string_view name, int width, int height, std::uint32_t window_flags);
sdl_renderer create_renderer(SDL_Window * window);
sdl_surface load_bitmap(std::string_view path);

constexpr lookup_table<SDL_InitFlags, std::string_view, 8> subsystem_flag_names
{
    { SDL_INIT_AUDIO,       "audio" },
    { SDL_INIT_VIDEO,       "video" },
    { SDL_INIT_JOYSTICK,    "joystick" },
    { SDL_INIT_HAPTIC,      "haptic" },
    { SDL_INIT_GAMEPAD,     "gamepad" },
    { SDL_INIT_EVENTS,      "events" },
    { SDL_INIT_SENSOR,      "sensor" },
    { SDL_INIT_CAMERA,      "camera" },
};

constexpr lookup_table<SDL_WindowFlags, std::string_view, 26> window_flag_names
{
    { SDL_WINDOW_FULLSCREEN,          "fullscreen" },
    { SDL_WINDOW_OPENGL,              "opengl" },
    { SDL_WINDOW_OCCLUDED,            "occluded" },
    { SDL_WINDOW_HIDDEN,              "hidden" },

    { SDL_WINDOW_BORDERLESS,          "borderless" },
    { SDL_WINDOW_RESIZABLE,           "resizable" },
    { SDL_WINDOW_MINIMIZED,           "minimized" },
    { SDL_WINDOW_MAXIMIZED,           "maximized" },

    { SDL_WINDOW_MOUSE_GRABBED,       "mouse grabbed" },
    { SDL_WINDOW_INPUT_FOCUS,         "input focus" },
    { SDL_WINDOW_MOUSE_FOCUS,         "mouse focus" },
    { SDL_WINDOW_EXTERNAL,            "external" },

    { SDL_WINDOW_MODAL,               "modal" },
    { SDL_WINDOW_HIGH_PIXEL_DENSITY,  "high pixel density" },
    { SDL_WINDOW_MOUSE_CAPTURE,       "mouse capture" },
    { SDL_WINDOW_MOUSE_RELATIVE_MODE, "mouse relative mode" },

    { SDL_WINDOW_ALWAYS_ON_TOP,       "always on top" },
    { SDL_WINDOW_UTILITY,             "utility" },
    { SDL_WINDOW_TOOLTIP,             "tooltip" },
    { SDL_WINDOW_POPUP_MENU,          "popup menu" },

    { SDL_WINDOW_KEYBOARD_GRABBED,    "keyboard grabbed" },
    { SDL_WINDOW_FILL_DOCUMENT,       "fill document" },
    { SDL_WINDOW_VULKAN,              "vulkan" },
    { SDL_WINDOW_METAL,               "metal" },

    { SDL_WINDOW_TRANSPARENT,         "transparent" },
    { SDL_WINDOW_NOT_FOCUSABLE,       "not focusable" }
};

struct project_settings
{
    std::string name = "ion Project";
};

struct engine_settings
{
    std::string installation_directory = "C:/msys64/mingw64";
    SDL_InitFlags subsystem_flags = SDL_INIT_VIDEO;
};

struct window_settings
{
    std::optional<std::string> name = std::nullopt;
    std::uint32_t width = 640u;
    std::uint32_t height = 480u;
    SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE;
};
}