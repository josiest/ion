#pragma once

#include <ion/sdl/window.hpp>
#include <SDL2/SDL.h>

#include <entt/entity/registry.hpp>
#include "components.hpp"

namespace systems {

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
    auto munchies = registry.view<component::bbox, component::color>();

    munchies.each([renderer](auto const & box, auto const & color) {
        // draw the munchy with the appropriate color
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xff);

        // cast the bbox to an SDL_Rect and draw the munchy
        auto const rect = static_cast<SDL_Rect>(box);
        SDL_RenderFillRect(renderer, &rect);
    });
}
}
