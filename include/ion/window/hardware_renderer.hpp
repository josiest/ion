#pragma once

#include "ion/isotope.hpp"
#include <SDL.h>

#include <cstdint>
#include <string>

namespace ion {

/** An SDL_Window with a renderer */
class hardware_renderer : public isotope {
public:
    // delete any unwanted implicit constructors
    hardware_renderer() = delete;
    hardware_renderer(hardware_renderer const &) = delete;

    /**
     * Create a render window with already initialized resources
     *
     * \param window the underlying SDL_Window to use
     * \param renderer the underlying SDL_Renderer to use
     */
    hardware_renderer(SDL_Window * window, SDL_Renderer * renderer) noexcept;
    hardware_renderer(hardware_renderer && temp) noexcept;
    ~hardware_renderer();

    /**
     * Create a simple hardware-rendering window
     *
     * \param title the title of the window
     * \param width the width of the window
     * \param height the height of the window
     * \param window_flags the flags used to initialize the window with
     *
     * The resulting window will be in the position as implemented by
     * SDL_WINDOWPOS_UNDEFINED. The renderer will be hardware accelerated
     * without vsync and use the first acceptable graphics driver.
     */
    static hardware_renderer basic_window(
            std::string const & title,
            std::uint32_t width, std::uint32_t height,
            std::uint32_t window_flags = 0) noexcept;
    
    /** Cast this window to its underlying SDL_Window pointer */
    inline operator SDL_Window *() noexcept { return _window; }

    /** Cast this window to its underlying SDL_Renderer pointer */
    inline operator SDL_Renderer *() noexcept { return _renderer; }

    /**
     * Create a hardware-rendering window
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
    static hardware_renderer make_window(
            std::string const & title, int x, int y,
            std::uint32_t width, std::uint32_t height,
            std::uint32_t window_flags, int driver_index=-1,
            std::uint32_t render_flags = SDL_RENDERER_ACCELERATED) noexcept;
private:
    SDL_Window * _window;
    SDL_Renderer * _renderer;
};
}
