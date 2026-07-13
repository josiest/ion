#pragma once
#include "ion/containers/lookup_table.hpp"

#include <SDL3/SDL_init.h>

#include <string_view>
#include <yaml-cpp/yaml.h>

namespace YAML
{
class Node;
}

namespace ion
{
bool read_subsystem_flags(const YAML::Node & node, SDL_InitFlags & flags);
bool read_window_flags(const YAML::Node & node, SDL_WindowFlags & flags);

bool read_subsystem_flag(std::string_view src, SDL_InitFlags & flag);
bool read_window_flag(std::string_view src, SDL_WindowFlags & flag);

namespace internal
{
constexpr lookup_table<SDL_InitFlags, std::string_view, 8> subsystem_flags
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

constexpr lookup_table<SDL_WindowFlags, std::string_view, 26> window_flags
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
}
}