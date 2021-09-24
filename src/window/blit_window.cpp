#include "ion/window/blit_window.hpp"
#include <SDL.h>
#include <string>

namespace ion {

blit_window::blit_window(SDL_Window * window) noexcept
    : _window{window}
{
    // failure if null was given
    if (not window) {
        _error = "blit window initialized with a nullptr";
    }
}

blit_window::~blit_window() noexcept
{
    // window may exist if initialization failed
    // so only destroy if it does exist
    if (_window) {
        SDL_DestroyWindow(_window);
    }
    _window = nullptr;
}

blit_window::blit_window(std::string const & title,
                         std::uint32_t width, std::uint32_t height,
                         std::uint32_t flags) noexcept

      // default to using undefined position
    : blit_window(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                  width, height, flags)
{}

blit_window::blit_window(std::string const & title, int x, int y,
                         std::uint32_t width, std::uint32_t height,
                         std::uint32_t flags) noexcept
{
    // forward the construtor arguments to sdl call
    _window = SDL_CreateWindow(title.c_str(), x, y, width, height, flags);

    // use sdl's error if it fails
    if (not _window) {
        _error = SDL_GetError();
    }
}
}
