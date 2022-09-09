#pragma once

// frameworks
#include <SDL.h>

// data types
#include <string>
#include <cstdint>

// resource handles
#include <tl/expected.hpp>

namespace ion {

/**
 * \class window
 * \brief A resource handle for an SDL_Window
 *
 * \details `ion::window` manages an `SDL_Window *` resource through its
 * instance lifetime so that the primitive SDL window is destroyed when the ion
 * window goes out of scope.
 *
 * Can be used implicitly as an `SDL_Window *`:
 *
 * ```
 * ion::window window = // ... //
 * SDL_Surface * screen = nullptr;
 * SDL_GetWindowSurface(window, screen);
 * ```
 *
 * \warn implicit `SDL_Window *` should only be used with primitive SDL calls.
 *       In particular, creating an `SDL_Window *` from an `ion::window` static
 *       factory method such as `ion::window::at_anywhere` will result in
 *       undefined behavior.
 */
class window {
public:
    // remove unwanted implicit constructors
    window(window const &) = delete;
    window() = delete;

    // resource management
    window(window && temp);
    ~window();

    // static factories
    /**
     * \brief Create a window at a specific position on the screen.
     *
     * \param title the title of the window
     * \param x the x-coordinate of the upper-left corner
     * \param y the y-coordinate of the upper-left corner
     * \param width the width of the window
     * \param height the height of the window
     * \param flags used to initialize the window
     */
    static tl::expected<window, std::string>
    at_position(std::string const & title, int x, int y,
                std::uint32_t width, std::uint32_t height,
                std::uint32_t flags = 0);

    /**
     * \brief Create a window anywhere on the screen.
     *
     * \param title the title of the window
     * \param width the width of the window
     * \param height the height of the window
     * \param flags used to initialize the window
     */
    static tl::expected<window, std::string>
    at_anywhere(std::string const & title,
                std::uint32_t width, std::uint32_t height,
                std::uint32_t flags = 0);

    inline operator SDL_Window * () { return _window; }
private:
    window(SDL_Window * sdl_window);
    SDL_Window * _window;
};
}
