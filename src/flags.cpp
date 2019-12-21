#include "flags.hpp"
#include <SDL2/SDL.h>

unsigned int ion::window_state::to_uint(const ion::window_flags& flags)
{
    using namespace ion::window_state;
    unsigned int _flags = 0;
    if (flags[fullscreen]) {
        _flags |= SDL_WINDOW_FULLSCREEN;
    }
    if (flags[fullscreen_desktop]) {
        _flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    if (flags[opengl]) {
        _flags |= SDL_WINDOW_OPENGL;
    }
    if (flags[vulkan]) {
        _flags |= SDL_WINDOW_VULKAN;
    }
    if (flags[hidden]) {
        _flags |= SDL_WINDOW_HIDDEN;
    }
    if (flags[borderless]) {
        _flags |= SDL_WINDOW_BORDERLESS;
    }
    if (flags[resizable]) {
        _flags |= SDL_WINDOW_RESIZABLE;
    }
    if (flags[minimized]) {
        _flags |= SDL_WINDOW_MINIMIZED;
    }
    if (flags[maximized]) {
        _flags |= SDL_WINDOW_MAXIMIZED;
    }
    if (flags[input_grabbed]) {
        _flags |= SDL_WINDOW_INPUT_GRABBED;
    }
    if (flags[high_dpi]) {
        _flags |= SDL_WINDOW_ALLOW_HIGHDPI;
    }
    return _flags;
}

unsigned int ion::render_state::to_uint(const render_flags& flags)
{
    using namespace ion::render_state;
    unsigned int _flags = 0;
    if (flags[software]) {
        _flags |= SDL_RENDERER_SOFTWARE;
    }
    if (flags[accelerated]) {
        _flags |= SDL_RENDERER_ACCELERATED;
    }
    if (flags[vsync]) {
        _flags |= SDL_RENDERER_PRESENTVSYNC;
    }
    if (flags[target_texture]) {
        _flags |= SDL_RENDERER_TARGETTEXTURE;
    }
    return _flags;
}
