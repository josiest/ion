#include "ion/graphics/renderer.hpp"

namespace ion {

renderer::renderer(renderer && temp)
    : _renderer{temp._renderer}
{
    temp._renderer = nullptr;
}

renderer::~renderer()
{
    if (_renderer) {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }
}

renderer::renderer(SDL_Renderer * sdl_renderer)
    : _renderer{sdl_renderer}
{
}

tl::expected<renderer, std::string>
renderer::with_driver_at(SDL_Window * win, int index, std::uint32_t flags)
{
    if (auto * sdl_renderer = SDL_CreateRenderer(win, index, flags)) {
        return renderer{sdl_renderer};
    }
    return tl::unexpected(SDL_GetError());
}

tl::expected<renderer, std::string>
renderer::with_default_driver(SDL_Window * win, std::uint32_t flags)
{
    return renderer::with_driver_at(win, -1, flags);
}
}
