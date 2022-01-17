#pragma once

#include <concepts>
#include <SDL.h>

namespace ion {

/**
 * Convertible to an SDL_Window pointer
 */
template<class window_t>
concept window_resource = std::convertible_to<window_t, SDL_Window *>;

/**
 * Convertible to an SDL_Renderer pointer
 */
template<class renderer_t>
concept renderer_resource = std::convertible_to<renderer_t, SDL_Renderer *>;

/**
 * Convertible to an SDL_Window pointer and an SDL_Renderer pointer
 */
template<class window_t>
concept hardware_renderable =
    window_resource<window_t> && renderer_resource<window_t>;
}
