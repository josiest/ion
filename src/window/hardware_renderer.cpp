#include "ion/window/hardware_renderer.hpp"

#include <SDL2/SDL.h>
#include <cstdint>
#include <string>

using uint = std::uint32_t;

namespace ion {

hardware_renderer::hardware_renderer(
        SDL_Window * window, SDL_Renderer * renderer) noexcept

    : _window{window}, _renderer{renderer}
{
    // set the error message if window is null
    if (not window or not renderer) {
        set_error("hardware_renderer initialized with null window or renderer");
    }
}

hardware_renderer::hardware_renderer(hardware_renderer && window) noexcept
    : _window(window._window), _renderer(window._renderer)
{
    window._window = nullptr;
    window._renderer = nullptr;

    set_error(window.get_error());
    window.set_error("window has been moved to another object");
}

hardware_renderer::~hardware_renderer()
{
    if (_renderer) {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }
    if (_window) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
}

hardware_renderer hardware_renderer::basic_window(
        std::string const & title,
        uint width, uint height,
        uint window_flags) noexcept
{
    // initialize the window with position undefined
    // and default renderer settings
    return hardware_renderer::make_window(
            title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height, window_flags
            );
}

hardware_renderer hardware_renderer::make_window(
        std::string const & title, int x, int y,
        uint width, uint height, uint window_flags,
        int driver_index, uint render_flags) noexcept
{
    // initialize the window
    auto sdl_window = SDL_CreateWindow(
            title.c_str(), x, y, width, height, window_flags
            );
    // initialize the renderer
    auto renderer = SDL_CreateRenderer(
            sdl_window, driver_index, render_flags
            );

    hardware_renderer window(sdl_window, renderer);

    // set the error message and quit if window initialization failed
    if (not sdl_window) {
        window.set_error(SDL_GetError());
    }
    // set the error message and clean up if renderer initialization failed
    else if (not renderer) {
        window.set_error(SDL_GetError());
        SDL_DestroyWindow(window._window);
        window._window = nullptr;
    }
    return window;
}
}
