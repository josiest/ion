#pragma once

#include <SDL2/SDL_video.h>

#include <string>
#include <cstdint>

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
    explicit blit_window(SDL_Window * window) noexcept;

    /**
     * Create a simple blit window
     *
     * \param title the title of the window
     * \param width the width of the window
     * \param height the height of the window
     * \param flags used to initialize the window
     *
     * The position of the window will be as implemented by
     * SDL_WINDOWPOS_UNDEFINED.
     */
    blit_window(std::string const & title,
                std::uint32_t width, std::uint32_t height,
                std::uint32_t flags = 0) noexcept;

    /**
     * Create a blit window
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
    blit_window(std::string const & title, int x, int y,
                std::uint32_t width, std::uint32_t height,
                std::uint32_t flags = 0) noexcept;

    ~blit_window();

    /** The underyling SDL window */
    inline SDL_Window * sdl_window() noexcept { return _window; }

    /** Determine if the window resource initialized properly */
    inline bool is_ok() const noexcept { return _window != nullptr; }

    /** Get the error message explaining why initialization failed */
    inline std::string error() const noexcept { return _error; }
private:
    SDL_Window * _window;
    std::string _error;
};

}
