#pragma once
#include <entt/entity/registry.hpp>
#include <SDL3/SDL_render.h>
#include "components.hpp"

namespace systems {

void render_colored_box(SDL_Renderer * renderer, const component::bbox& box,
                                                 const component::color & color);

inline void render(SDL_Renderer * renderer, entt::registry & registry)
{
    // clear the screen with white
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(renderer);

    // render the game objects
    registry.view<component::bbox, component::color>()
            .each([&](const auto & box, const auto & color) { render_colored_box(renderer, box, color); });
    SDL_RenderPresent(renderer);
}

inline void render_colored_box(SDL_Renderer * renderer, const component::bbox& box,
                                                        const component::color & color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xff);
    const auto render_rect = static_cast<SDL_FRect>(box);
    SDL_RenderFillRect(renderer, &render_rect);
}
}
