#pragma once

#include <ion/window/resource.hpp>
#include <SDL2/SDL_render.h>

#include <entt/entity/registry.hpp>
#include "components.hpp"

namespace systems {

void render_colored_box(ion::hardware_renderable auto & window,
                        const component::bbox& box, const component::color & color);

void render(ion::hardware_renderable auto & window, entt::registry & registry)
{
    // clear the screen with white
    SDL_SetRenderDrawColor(window, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(window);

    // render the game objects
    registry.view<component::bbox, component::color>()
            .each([&](const auto & box, const auto & color) { render_colored_box(window, box, color); });
    SDL_RenderPresent(window);
}

void render_colored_box(ion::hardware_renderable auto & window,
                        const component::bbox& box, const component::color & color)
{
    SDL_SetRenderDrawColor(window, color.r, color.g, color.b, 0xff);
    const auto render_rect = static_cast<SDL_Rect>(box);
    SDL_RenderFillRect(window, &render_rect);
}
}
