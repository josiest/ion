#include "ion/graphics/renderer.hpp"

namespace ion {

renderer::renderer(SDL_Renderer * sdl_renderer)
    : _renderer{sdl_renderer} {}

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

tl::expected<renderer, std::string>
renderer::with_driver_at(SDL_Window * sdl_window, int index,
                         std::uint32_t flags)
{
    auto * sdl_renderer = SDL_CreateRenderer(sdl_window, index, flags);
    if (not sdl_renderer) {
        return tl::unexpected{SDL_GetError()};
    }
    return renderer{sdl_renderer};
}

tl::expected<renderer, std::string>
renderer::with_default_driver(SDL_Window * window, std::uint32_t flags)
{
    return renderer::with_driver_at(window, -1, flags);
}
}
