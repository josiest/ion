#pragma once

#include "ion/graphics/image.hpp"

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

#include <cstdint>
#include <string>
#include <stdexcept>

#include <unordered_map>

namespace ion {
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
    operator SDL_Window *() noexcept { return _window; }
private:
    SDL_Window * _window;
};

/**
 * Create an SDL_Window pointer
 *
 * \param title the title of the window
 * \param width the width of the window
 * \param height the height of the window
 * \param flags the window flags
 *
 * \return the window created from the given parameters
 * \throw std::runtime_error if the window couldn't be created
 */
SDL_Window * load_window(std::string const & title,
                         std::size_t width, std::size_t height,
                         std::uint32_t flags=0);

/**
 * Create an SDL_Window pointer
 *
 * \param title the title of the window
 * \param x the x-coordinate of the window position
 * \param y the y-coordinate of the window position
 * \param width the width of the window
 * \param height the height of the window
 * \param flags the window flags
 *
 * \return the window created from the given parameters
 * \throw std::runtime_error if the window couldn't be created
 */
SDL_Window * load_window(std::string const & title, int x, int y,
                         std::size_t width, std::size_t height,
                         std::uint32_t flags = 0);

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
 * Create a basic software-rendering window.
 */
blit_window basic_blit_window(std::string const & title,
                              size_t width, size_t height);
}
