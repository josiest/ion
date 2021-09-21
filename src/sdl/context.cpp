#include "ion/sdl/context.hpp"

#include <SDL2/SDL.h>
#include <cstdint>

namespace ion {

sdl_context::sdl_context() noexcept : sdl_context(SDL_INIT_VIDEO) {}
sdl_context::sdl_context(std::uint32_t flags) noexcept
{
    // set the error message if sdl couldn't initialize
    if (SDL_Init(flags) < 0) {
        _error = SDL_GetError();
    }
}

sdl_context::~sdl_context()
{
    // quit sdl if it was initialized
    if (is_ok()) {
        SDL_Quit();
    }
}

std::string sdl_context::error() const noexcept
{
    return _error;
}
bool sdl_context::is_ok() const noexcept
{
    return _error.empty();
}
}
