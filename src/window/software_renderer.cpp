#include "ion/window/software_renderer.hpp"
#include <SDL.h>

// datatypes
#include <string>
#include <cstdint>

// aliases
using uint = std::uint32_t;

namespace ion {

software_renderer::software_renderer(SDL_Window * window) noexcept
    : _window{window}
{
    // failure if null was given
    if (not window) {
        set_error("blit window initialized as null");
    }
}

software_renderer::~software_renderer() noexcept
{
    // window may exist if initialization failed
    // so only destroy if it does exist
    if (_window) {
        SDL_DestroyWindow(_window);
    }
    _window = nullptr;
}

software_renderer software_renderer::basic_window(
        std::string const & title,
        uint width, uint height, uint flags) noexcept
{
    return software_renderer::make_window(
            title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height, flags
            );
}

software_renderer software_renderer::make_window(
        std::string const & title, int x, int y,
        uint width, uint height, uint flags) noexcept

{
    auto sdl_window = SDL_CreateWindow(
            title.c_str(), x, y,

            // sdl interface uses signed ints for width and height
            // ion interface uses unsigned since negative width/height doesn't
            // make much sense

            // it's highly unlikely that they'll be large enough to lose
            // information when casting from unsigned, so let the compiler know
            // it's okay by using static_cast
            static_cast<int>(width),
            static_cast<int>(height), flags
            );

    software_renderer window(sdl_window);
    if (not sdl_window) {
        window.set_error(SDL_GetError());
    }
    return window;
}
}
