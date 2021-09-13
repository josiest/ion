#include "ion/graphics/window.hpp"
#include <SDL2/SDL.h>
#include <string>

#include <stdexcept>
#include <sstream>

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

// helper functions for creating windows
SDL_Window *
load_window(std::string const & title, size_t width, size_t height,
            int x, int y, uint32_t flags)
{
    // initialize video if it hasn't already been done
    if (!SDL_WasInit(SDL_INIT_VIDEO) && SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        std::stringstream message;
        message << "SDL couldn't initialize video! SDL_Error: "
                << SDL_GetError();
        throw std::runtime_error{message.str()};
    }

    // create the window and a renderer for it
    SDL_Window * window_ptr = SDL_CreateWindow(
            title.c_str(), x, y,
            static_cast<int>(width),
            static_cast<int>(height), flags
            );

    if (!window_ptr) {
        std::stringstream message;
        message << "Window couldn't be created! SDL_Error: "
                << SDL_GetError();
        throw std::runtime_error{message.str()};
    }
    return window_ptr;
}

SDL_Renderer * load_renderer(SDL_Window * window_ptr,
                             int driver_index, uint32_t flags)
{
    // make sure the given window pointer isn't null
    if (!window_ptr) {
        throw std::invalid_argument{"window_ptr must not be null!"};
    }

    // create the renderer
    SDL_Renderer * renderer =
        SDL_CreateRenderer(window_ptr, driver_index, flags);

    // error if the renderer couldn't be created (likely a shortage of memory)
    if (!renderer) {
        std::stringstream message;
        message << "Window renderer couldn't be created! SDL_Error: "
                << SDL_GetError();
        SDL_DestroyWindow(window_ptr); // clean up
        throw std::runtime_error{message.str()};
    }
    return renderer;
}

window basic_window(std::string const & title, size_t width, size_t height)
{
    // create the window, then create the renderer from the window
    auto window_ptr = load_window(title, width, height);
    return window(window_ptr, load_renderer(window_ptr));
}

window basic_blit_window(std::string const & title, size_t width, size_t height)
{
    return window(load_window(title, width, height), nullptr);
}

}
