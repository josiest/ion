#pragma once

// frameworks
#include <SDL.h>

// data types
#include <string>
#include <cstdint>

// resource handles
#include <tl/expected.hpp>

namespace ion {

/** A resource handle for an SDL_Window. */
class window {
public:
    // delete unwanted implicit constructors
    window() = delete;
    window(window const &) = delete;

    // resource management
    window(window && temp);
    ~window();

    // static factories

    /**
     * \brief Create a window at a specific position on the screen.
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
     *
     * \return the window if it was made succesfully, otherwise a message for
     *         what went wrong when creating the window
     */
    static tl::expected<window, std::string>
    at_position(std::string const & title, int x, int y,
                std::uint32_t width, std::uint32_t height,
                std::uint32_t flags = 0);

    /**
     * \brief Create a window at an undefined position on the screen.
     *
     * \param title the title of the window
     *
     * \param width the width of the window
     * \param height the height of the window
     *
     * \param flags used to initialize the window
     *
     * \return the window if it was made succesfully, otherwise a message for
     *         what went wrong when creating the window
     */
    static tl::expected<window, std::string>
    at_anywhere(std::string const & title, std::uint32_t width,
                std::uint32_t height, std::uint32_t flags = 0);

    inline operator SDL_Window *() { return _window; }
private:
    window(SDL_Window * sdl_window);
    SDL_Window * _window;
};
}
