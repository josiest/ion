#pragma once

#include <ion/sdl/window.hpp>
#include <SDL2/SDL.h>

#include <entt/entity/registry.hpp>
#include "components.hpp"

void render_munchies(ion::renderable_window auto & window,
                     entt::registry & registry);

void render(ion::renderable_window auto & window, entt::registry & registry)
{
    // clear the screen with white
    auto renderer = window.sdl_renderer();
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(renderer);

    // render the game objects
    render_munchies(window, registry);
    SDL_RenderPresent(renderer);
}

void render_munchies(ion::renderable_window auto & window,
                     entt::registry & registry)
{
    auto renderer = window.sdl_renderer();

    auto view = registry.view<Position, Size, Color>();
    view.each([renderer](auto const & p, auto const & size, auto const & color) {

        // get the upper-left corner of the munchy square
        auto x = static_cast<int>(std::round(p.x - size.value/2.f));
        auto y = static_cast<int>(std::round(p.y - size.value/2.f));
        SDL_Rect const munchy{x, y, size.value, size.value};

        // draw the munchy with the appropriate color
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &munchy);
    });
}
