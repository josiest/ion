#pragma once

#include "ion/isotope.hpp"
#include <SDL.h>

#include <string>
#include <cstdint>

namespace ion {

/**
 * A window that uses software-rendering.
 */
class software_renderer : public isotope {
public:
    // delete unwanted implicit constructors
    software_renderer() = delete;
    software_renderer(software_renderer const &) = delete;

    /**
     * Create a software-rendering window from an SDL_Window
     *
     * \param window the underlying SDL_Window
     * \param img_init_flags the flags to initialize SDL_image with
     */
    explicit software_renderer(SDL_Window * window) noexcept;
    software_renderer(software_renderer && window) noexcept;

    /**
     * Create a simple software-rendering window
     *
     * \param title the title of the window
     * \param width the width of the window
     * \param height the height of the window
     * \param flags used to initialize the window
     *
     * The position of the window will be as implemented by
     * SDL_WINDOWPOS_UNDEFINED.
     */
    static software_renderer basic_window(
            std::string const & title,
            std::uint32_t width, std::uint32_t height,
            std::uint32_t flags = 0) noexcept;

    /**
     * Create a software-rendering window
     *
     * \param title the title of the window
     *
     * \param x the x-coordinate of the window position
     * \param y the y-coordinate of the window position
     *
     * \param width the width of the window
     * \param height the height of the window
     *
     * \param flags used to initialize the window
     */
    static software_renderer make_window(
            std::string const & title, int x, int y,
            std::uint32_t width, std::uint32_t height,
            std::uint32_t flags = 0) noexcept;

    ~software_renderer();

    /** The underyling SDL window */
    inline operator SDL_Window *() noexcept { return _window; }
private:
    SDL_Window * _window;
};

}
