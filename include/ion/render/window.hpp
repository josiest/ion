#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace ion {

/**
 * A wrapper class for an SDL_Window
 */
class Window {
public:
    /**
     * Create a window handler object using the given window and renderer
     */
    Window(SDL_Window * window, SDL_Renderer * renderer);
    ~Window();

    /**
     * Get the underlying SDL_Window pointer
     */
    inline SDL_Window * sdl_window() { return _window; }

    /**
     * Get the underlying SDL_Renderer pointer
     */
    inline SDL_Renderer * sdl_renderer() { return _renderer; }

    /**
     * Get the undelrying SDL_Surface of the window
     */
    inline SDL_Surface * sdl_surface() { return SDL_GetWindowSurface(_window); }
private:
    SDL_Window * _window;
    SDL_Renderer * _renderer;
};

/**
 * Create a basic renderer-accelerated window.
 */
Window basic_window(std::string const & title, size_t width, size_t height);

/**
 * Create a basic software-rendering window.
 */
Window basic_blit_window(std::string const & title, size_t width, size_t height);
}
