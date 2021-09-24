#pragma once

#include <ion/window/resource.hpp>
#include <SDL2/SDL.h>

#include <entt/entity/registry.hpp>
#include "components.hpp"

namespace systems {

void render_munchies(ion::renderable_window auto & window,
                     entt::registry & registry);

void render(ion::renderable_window auto & window, entt::registry & registry)
{
    // clear the screen with white
    SDL_SetRenderDrawColor(window, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(window);

    // render the game objects
    render_munchies(window, registry);
    SDL_RenderPresent(window);
}

void render_munchies(ion::renderable_window auto & window,
                     entt::registry & registry)
{
    auto munchies = registry.view<component::bbox, component::color>();
    munchies.each([&window](auto const & box, auto const & color) {
        // draw the munchy with the appropriate color
        SDL_SetRenderDrawColor(window, color.r, color.g, color.b, 0xff);

        // cast the bbox to an SDL_Rect and draw the munchy
        auto const rect = static_cast<SDL_Rect>(box);
        SDL_RenderFillRect(window, &rect);
    });
}
}
