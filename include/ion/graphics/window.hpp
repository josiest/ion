#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace ion {

/**
 * A wrapper class for an SDL_Window
 */
class window {
public:
    window() = delete;

    /**
     * Create a window handler object using the given window and renderer
     */
    window(SDL_Window * window_ptr, SDL_Renderer * renderer);
    ~window();

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
 * Create an SDL_Window pointer
 *
 * \param title the title of the window
 * \param width the width of the window
 * \param height the height of the window
 *
 * \param x the x-coordinate of the window position
 * \param y the y-coordinate of the window position
 * \param flags the window flags
 *
 * \return the window created from the given parameters
 *
 * \throw std::runtime_error if video couldn't initialize or if the window
 *        couldn't be created
 */
SDL_Window * load_window(std::string const & title, size_t width, size_t height,
                         int x=SDL_WINDOWPOS_UNDEFINED,
                         int y=SDL_WINDOWPOS_UNDEFINED,
                         uint32_t flags=0);

/**
 * Create a renderer from an SDL_Window
 *
 * \param window_ptr the SDL_Window to create a renderer for. If the renderer
 *                   fails to intialize properly, this pointer will be
 *                   invalidated and its memory freed.
 *
 * \param driver_index the index of the driver to use. Defaults to using the
 *                     first acceptable driver
 *
 * \param flags the render flags. Defaults to using hardware acceleration with
 *              no vsync
 *
 * \return the SDL_Renderer pointer that was created
 *
 * \throw std::invalid_argument if window_ptr is null
 * \throw std::runtime_error if the renderer couldn't be created
 */
SDL_Renderer * load_renderer(SDL_Window * window_ptr,
                             int driver_index=-1,
                             uint32_t flags=SDL_RENDERER_ACCELERATED);

/**
 * Create a basic render-accelerated window
 *
 * \param title the title of the window
 * \param width the width of the window
 * \param height the height of the window
 *
 * \return the window object
 */
window basic_window(std::string const & title, size_t width, size_t height);

/**
 * Create a basic software-rendering window.
 */
window basic_blit_window(std::string const & title, size_t width, size_t height);
}
