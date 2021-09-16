#pragma once

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

#include <string>
#include <stdexcept>

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
concept renderable_window = window_resource<window_t> && renderer_resource<window_t>;

/**
 * A render-less window meant to present by blitting surfaces.
 */
class blit_window {
public:
    /**
     * An owning reference to SDL_Surfaces
     */

    // delete unwanted implicit constructors
    blit_window() = delete;
    blit_window(blit_window const &) = delete;

    /**
     * Create a render-less window from an SDL_Window
     *
     * \param window the underlying SDL_Window
     * \param img_init_flags the flags to initialize SDL_image with
     */
    blit_window(SDL_Window * window, int img_init_flags = 0);

    ~blit_window();
    operator SDL_Window *() { return _window; }
private:
    SDL_Window * _window;
};

/**
 * A window with a renderer
 */
class render_window {
public:
    // delete unwanted implicit constructors
    render_window() = delete;
    render_window(render_window const &) = delete;

    /**
     * Create a window with a renderer
     *
     * \param window the underlying SDL_Window to use
     * \param renderer the underlying SDL_Renderer to use
     * \param init_img_flags the flags to initialize SDL_image with
     *
     * \throws std::runtime_error if SDL couldn't initialize video or image
     */
    render_window(SDL_Window * window, SDL_Renderer * renderer,
                  int init_img_flags = 0);
    ~render_window();

    /** Convert the window object to its underlying SDL_Window */
    operator SDL_Window *() { return _window; }

    /** Convert the window object to its underlying SDL_Renderer */
    operator SDL_Renderer *() { return _renderer; }
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
 * \throw std::runtime_error if the window couldn't be created
 */
SDL_Window * load_window(std::string const & title, size_t width, size_t height,
                         int x=SDL_WINDOWPOS_UNDEFINED,
                         int y=SDL_WINDOWPOS_UNDEFINED,
                         uint32_t flags=0);

/**
 * Create a renderer from an SDL_Window
 *
 * \param window the SDL_Window to create a renderer for. If the renderer
 *               fails to intialize properly, this pointer will be
 *               invalidated and its memory freed.
 *
 * \param driver_index the index of the driver to use. Defaults to using the
 *                     first acceptable driver
 *
 * \param flags the render flags. Defaults to using hardware acceleration with
 *              no vsync
 *
 * \return the SDL_Renderer pointer that was created
 *
 * \throw std::invalid_argument if window is null
 * \throw std::runtime_error if the renderer couldn't be created
 */
SDL_Renderer * load_renderer(SDL_Window * window,
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
render_window basic_window(std::string const & title,
                           size_t width, size_t height);

/**
 * Create a basic software-rendering window.
 */
blit_window basic_blit_window(std::string const & title,
                              size_t width, size_t height);
}
