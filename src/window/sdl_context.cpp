#include "ion/window/sdl_context.hpp"

#include <SDL.h>
#include <cstdint>

namespace ion {

sdl_context::sdl_context() noexcept : sdl_context(SDL_INIT_VIDEO) {}
sdl_context::sdl_context(std::uint32_t flags) noexcept
{
    // set the error message if sdl couldn't initialize
    if (SDL_Init(flags) < 0) {
        set_error(SDL_GetError());
    }
}

sdl_context::~sdl_context()
{
    // quit sdl if it was initialized
    if (get_error().empty()) {
        SDL_Quit();
    }
}
}
