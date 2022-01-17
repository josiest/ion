#pragma once

#include "ion/isotope.hpp"
#include <SDL.h>

#include <cstdint>
#include <string>

namespace ion {

/** An SDL_Window with a renderer */
class render_window : public isotope {
public:
    // delete any unwanted implicit constructors
    render_window() = delete;
    render_window(render_window const &) = delete;

    /**
     * Create a render window with already initialized resources
     *
     * \param window the underlying SDL_Window to use
     * \param renderer the underlying SDL_Renderer to use
     */
    render_window(SDL_Window * window, SDL_Renderer * renderer) noexcept;

    /**
     * Create a simple render window
     *
     * \param title the title of the window
     * \param width the width of the window
     * \param height the height of the window
     * \param window_flags the flags used to initialize the window with
     *
     * The resulting window will be in the position as implemeneted by
     * SDL_WINDOWPOS_UNDEFINED. The renderer will be hardware accelerated
     * without vsync and use the first acceptable graphics driver.
     */
    render_window(std::string const & title,
                  std::size_t width, std::size_t height,
                  std::uint32_t window_flags = 0) noexcept;
    
    /**
     * Create a render window
     *
     * \param title the title of the window
     *
     * \param x the x-coordinate of the window position
     * \param y the y-coordinate of the window position
     *
     * \param width the width of the window
     * \param height the height of the window
     *
     * \param window_flags the flags used to initialize the window
     * \param driver_index the index of the graphics driver to use
     * \param renderer_flags the flags used to initialize the renderer
     */
    render_window(std::string const & title, int x, int y,
                  std::size_t width, std::size_t height,
                  std::uint32_t window_flags, int driver_index=-1,
                  std::size_t render_flags = SDL_RENDERER_ACCELERATED) noexcept;
    
    ~render_window();

    /** Cast this window to its underlying SDL_Window pointer */
    inline operator SDL_Window *() noexcept { return _window; }

    /** Cast this window to its underlying SDL_Renderer pointer */
    inline operator SDL_Renderer *() noexcept { return _renderer; }
private:
    SDL_Window * _window;
    SDL_Renderer * _renderer;
};
}
