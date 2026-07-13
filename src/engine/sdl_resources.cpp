#include "ion/engine/sdl_resources.hpp"

#include <SDl3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>

ion::internal::sdl_system::sdl_system(std::uint32_t init_flags)
    : was_init_(SDL_Init(init_flags))
{
}

void ion::internal::sdl_deleter::operator()(const sdl_system * sdl) const
{
    if (sdl->was_init()) { SDL_Quit(); }
}

void ion::internal::sdl_deleter::operator()(SDL_Window * window) const
{
    if (window) { SDL_DestroyWindow(window); }
}

void ion::internal::sdl_deleter::operator()(SDL_Renderer * renderer) const
{
    if (renderer) { SDL_DestroyRenderer(renderer); }
}

ion::sdl_system ion::init_sdl(std::uint32_t init_flags)
{
    return sdl_system(new internal::sdl_system(init_flags), {});
}

ion::sdl_window ion::create_window(std::string_view name, int width, int height, std::uint32_t window_flags)
{
    return sdl_window(SDL_CreateWindow(name.data(), width, height, window_flags), {});
}

ion::sdl_renderer ion::create_renderer(SDL_Window * window)
{
    return sdl_renderer(SDL_CreateRenderer(window, nullptr), {});
}
