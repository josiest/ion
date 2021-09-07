#include "window.hpp"
#include "video.hpp"

#include <memory>
#include <string>

namespace ion {

Window::Window(SDL_Window * window, SDL_Renderer * renderer)
    : _window(window), _renderer(renderer) {}

Window::~Window()
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

std::unique_ptr<Window>
unique_basic_window(std::string const & title, size_t width, size_t height)
{
    // make sure video is initialized before making windows
    init_video();

    SDL_Window * window = SDL_CreateWindow(
            title.c_str(),
            // use default ("undefined") window position
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height,
            0); // no flags: basic window

    SDL_Renderer * renderer = SDL_CreateRenderer(
            window,
            -1, // grab the first acceptable renderer
            SDL_RENDERER_ACCELERATED);

    return std::make_unique<Window>(window, renderer);
}

}
