#include "ion/window/sdl_context.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <cstdint>
#include <ranges>

namespace ion {

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
    if (not *this) {
        return;
    }
    cleanup_everything();

}

void sdl_context::init_ttf() noexcept
{
    // make sure sdl was properly initialed before initializing anything else
    if (not *this) {
        return;
    }
    if (TTF_Init() < 0) {
        set_error(TTF_GetError());
        cleanup_everything();
        return;
    }
    subsystem_destructors.push_back(&TTF_Quit);
}

void sdl_context::cleanup_everything() {
    // clean up subsystems in reverse order they were initialized
    for (auto cleanup : subsystem_destructors | std::views::reverse) {
        cleanup();
    }
    SDL_Quit();
}
}
