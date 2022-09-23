#include "ion/graphics/window.hpp"

namespace ion {

window::window(SDL_Window * sdl_window)
    : _window{ sdl_window }
{
}

window::window(window && temp)
    : _window{ temp._window }
{
    temp._window = nullptr;
}

window::~window()
{
    if (_window) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
}

tl::expected<window, std::string>
window::at_position(std::string const & title, int x, int y,
                    std::uint32_t width, std::uint32_t height,
                    std::uint32_t flags)
{
    auto * sdl_window = SDL_CreateWindow(
            title.c_str(), x, y, width, height, flags);
    if (not sdl_window) {
        return tl::unexpected( SDL_GetError() );
    }
    return window{ sdl_window };
}

tl::expected<window, std::string>
window::at_anywhere(std::string const & title,
                    std::uint32_t width, std::uint32_t height,
                    std::uint32_t flags)
{
    return window::at_position(
            title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height, flags);
}
}
