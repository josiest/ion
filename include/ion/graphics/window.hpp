#pragma once

#include "ion/graphics/image.hpp"

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

#include <string>
#include <stdexcept>

#include <unordered_map>

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
concept renderable_window =
    window_resource<window_t> && renderer_resource<window_t>;

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
 * A window with a renderer
 */
template<typename id_type=size_t, class texture_ptr=unique_texture>
class render_window {
public:
    // delete unwanted implicit constructors
    render_window() = delete;
    render_window(render_window const &) = delete;

    /** A map of texture ids to to pointers of SDL_Textures */
    std::unordered_map<id_type, texture_ptr> textures;

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
                  int img_init_flags = 0)

        : _window{window}, _renderer{renderer}
    {
        init_video();
        init_image(img_init_flags);
    }

    ~render_window()
    {
        // destroy all textures loaded with the renderer first
        textures.clear();

        // then destroy and clear the renderer
        if (_renderer) {
            SDL_DestroyRenderer(_renderer);
            _renderer = nullptr;
        }

        // finally destroy and clear the window pointer
        if (_window) {
            SDL_DestroyWindow(_window);
            _window = nullptr;
        }
    }

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
                         int x = SDL_WINDOWPOS_UNDEFINED,
                         int y = SDL_WINDOWPOS_UNDEFINED,
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
template<typename id_type=size_t, class texture_ptr=unique_texture>
render_window<id_type, texture_ptr>
basic_window(std::string const & title, size_t width, size_t height)
{
    // create the window, then create the renderer from the window
    auto window_ptr = load_window(title, width, height);
    using window_t = render_window<id_type, texture_ptr>;
    return window_t(window_ptr, load_renderer(window_ptr));
}

/**
 * Create a basic software-rendering window.
 */
blit_window basic_blit_window(std::string const & title,
                              size_t width, size_t height);
}
