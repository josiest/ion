#pragma once

// frameworks
#include <SDL.h>
#include <raisin/raisin.hpp>

// data types
#include <string>
#include <cstdint>

// resource handles
#include <tl/expected.hpp>

// algorithms
#include <iterator>

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

    /**
     * \brief Create a window from a config file.
     *
     * \param path the path of the config file
     * \param invalid_names a place to write invalid names to
     *
     * All window parameters must be found under a "window" table.
     * Parameters for the window:
     *
     *  [window]
     *  title = <string>
     *  x = [<int>]
     *  y = [<int>]
     *  width = <int>
     *  height = <int>
     *  flags = [<string>...]
     *
     * Acceptable flag names from a config file:
     *
     *  fullscreen:             SDL_WINDOW_FULLSCREEN
     *  fullscreen-desktop:     SDL_WINDOW_FULLSCREEN_DESKTOP
     *  opengl:                 SDL_WINDOW_OPENGL
     *  vulkan:                 SDL_WINDOW_VULKAN
     *  metal:                  SDL_WINDOW_METAL
     *  hidden:                 SDL_WINDOW_HIDDEN
     *  borderless:             SDL_WINDOW_BORDERLESS
     *  resizable:              SDL_WINDOW_RESIZABLE
     *  minimized:              SDL_WINDOW_MINIMIZED
     *  maximized:              SDL_WINDOW_MAXIMIZED
     *  input-grabbed:          SDL_WINDOW_INPUT_GRABBED
     *  allow-high-dpi:         SDL_WINDOW_ALLOW_HIGH_DPI
     *  shown:                  SDL_WINDOW_SHOWN
     *
     * example:
     *
     *  [window]
     *  title = "A Simple Example"
     *  width = 640
     *  height = 480
     */
    template<std::weakly_incrementable name_output>
    static tl::expected<window, std::string>
    from_config(std::string const & path, name_output invalid_names);

    inline operator SDL_Window * () const { return _window; }
private:
    window(SDL_Window * sdl_window);
    SDL_Window * _window;
};

//
// Template Implementation
//

template<std::weakly_incrementable name_output>
tl::expected<window, std::string>
window::from_config(std::string const & path, name_output invalid_names)
{
    auto window_result = raisin::make_window_from_config(path, invalid_names);
    if (not window_result) {
        return tl::unexpected(window_result.error());
    }
    return window{std::move(window_result).value()};
}
}
