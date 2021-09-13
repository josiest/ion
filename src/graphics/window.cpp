#include "ion/graphics/window.hpp"

#include <memory>
#include <string>
#include <stdexcept>

namespace ion {

window::window(SDL_Window * window_ptr, SDL_Renderer * renderer)
    : _window(window_ptr), _renderer(renderer) {}

window::~window()
{
    // destroy and clear the renderer pointer first
    if (_renderer) {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }

    // destroy and clear the window pointer last
    if (_window) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
}

window basic_window(std::string const & title, size_t width, size_t height)
{
    // initialize video if it hasn't already been done
    if (!SDL_WasInit(SDL_INIT_VIDEO) && SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        throw std::runtime_error{SDL_GetError()};
    }

    // create the window and a renderer for it
    SDL_Window * window_ptr = SDL_CreateWindow(
            title.c_str(),
            // use default ("undefined") window position
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height,
            0); // no flags: basic window

    if (!window_ptr) {
        throw std::runtime_error{SDL_GetError()};
    }

    SDL_Renderer * renderer = SDL_CreateRenderer(
            window_ptr,
            -1, // grab the first acceptable renderer
            SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        SDL_DestroyWindow(window_ptr);
        throw std::runtime_error{SDL_GetError()};
    }
    return window(window_ptr, renderer);
}

window basic_blit_window(std::string const & title, size_t width, size_t height)
{
    // initialize video if it hasn't already been done
    if (!SDL_WasInit(SDL_INIT_VIDEO) && SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        throw std::runtime_error{SDL_GetError()};
    }

    // create the window and a renderer for it
    SDL_Window * window_ptr = SDL_CreateWindow(
            title.c_str(),
            // use default ("undefined") window position
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height,
            SDL_WINDOW_SHOWN); // make sure the window renders

    if (!window_ptr) {
        throw std::runtime_error{SDL_GetError()};
    }

    return window(window_ptr, nullptr);
}

}
