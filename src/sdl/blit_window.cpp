#include "ion/sdl/blit_window.hpp"
#include <SDL.h>
#include <string>

namespace ion {

blit_window::blit_window(SDL_Window * window) noexcept
    : _window{window}
{
    if (not window) {
        _error = "blit window initialized with a nullptr";
    }
}

blit_window::~blit_window() noexcept
{
    if (_window) {
        SDL_DestroyWindow(_window);
    }
}

blit_window::blit_window(std::string const & title,
                         std::uint32_t width, std::uint32_t height,
                         std::uint32_t flags) noexcept

    : blit_window(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                  width, height, flags)
{}

blit_window::blit_window(std::string const & title, int x, int y,
                         std::uint32_t width, std::uint32_t height,
                         std::uint32_t flags) noexcept
{
    _window = SDL_CreateWindow(title.c_str(), x, y, width, height, flags);
    if (not _window) {
        _error = SDL_GetError();
    }
}
}
