#pragma once

#include <concepts>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

namespace ion {

/**
 * Convertible to an SDL_Window pointer
 */
template<class window_t>
concept sdl_window_resource = requires(window_t window) {
    { window.sdl_window() } -> std::same_as<SDL_Window *>;
};

/**
 * Convertible to an SDL_Renderer pointer
 */
template<class renderer_t>
concept renderer_resource = requires(renderer_t renderer) {
    { renderer.sdl_renderer() } -> std::same_as<SDL_Renderer *>;
};

/**
 * Convertible to an SDL_Window pointer and an SDL_Renderer pointer
 */
template<class window_t>
concept renderable_window =
    sdl_window_resource<window_t> && renderer_resource<window_t>;
}
