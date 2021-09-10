#include "systems/render.hpp"
#include "components.hpp"

void render(ion::Window & window, entt::registry & registry)
{
    auto renderer = window.sdl_renderer();
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

    SDL_RenderClear(renderer);
    render_munchies(window, registry);
    SDL_RenderPresent(renderer);
}

void render_munchies(ion::Window & window, entt::registry & registry)
{
    auto renderer = window.sdl_renderer();
    auto view = registry.view<Position, Size, Color>();
    view.each([renderer](auto const & pos, auto const & size,
                         auto const & color) {

        auto x = static_cast<int>(std::round(pos.x - size.value/2.f));
        auto y = static_cast<int>(std::round(pos.y - size.value/2.f));
        SDL_Rect const munchy{x, y, size.value, size.value};

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &munchy);
    });
}
