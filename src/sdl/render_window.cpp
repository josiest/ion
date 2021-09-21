#include "ion/sdl/render_window.hpp"

#include <SDL.h>
#include <cstdint>
#include <string>

namespace ion {

render_window::render_window(SDL_Window * window,
                             SDL_Renderer * renderer) noexcept

    : _window{window}, _renderer{renderer}
{
    // set the error message if window is null
    if (not window) {
        _error = "window initialized with null";
    }
    // update the error message if renderer is null
    if (not renderer) {

        // if both are null, make sure to say it in a neat way
        if (not window) {
            _error += " and ";
        }
        _error += "renderer initialized with null";
    }
}

render_window::render_window(std::string const & title,
                             std::size_t width, std::size_t height,
                             std::uint32_t window_flags) noexcept

    // initialize the window with position undefined
    // and default renderer settings
    : render_window{title,
                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                    width, height, window_flags}
{}

render_window::render_window(
        std::string const & title, int x, int y,
        std::size_t width, std::size_t height, std::uint32_t window_flags,
        int driver_index, std::size_t render_flags) noexcept
{
    // initialize the window
    _window = SDL_CreateWindow(title.c_str(), x, y, width, height, window_flags);

    // set the error message and quit if window initialization failed
    if (not _window) {
        _error = SDL_GetError();
        return;
    }

    // initialize the renderer
    _renderer = SDL_CreateRenderer(_window, driver_index, render_flags);

    // set the error message and clean up if renderer initialization failed
    if (not _renderer) {
        _error = SDL_GetError();
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
}

render_window::~render_window()
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

bool render_window::is_ok() const noexcept
{
    return _window != nullptr and _renderer != nullptr;
}

std::string render_window::error() const noexcept {
    return _error;
}
}
