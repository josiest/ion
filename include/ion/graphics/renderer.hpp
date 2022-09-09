#pragma once

// frameworks
#include <SDL.h>

// data types
#include <cstdint>
#include <string>

// resource handles
#include <tl/expected.hpp>

namespace ion {

/**
 * \class renderer
 * \brief A resource handle for an SDL_Renderer
 *
 * \details An `ion::renderer` manages an `SDL_Renderer *` resource trough its
 * instance lifetime so that the primitive SDL renderer is destroyed when the
 * ion renderer leaves scope.
 *
 * Can be used implictly as an `SDL_Renderer *`:
 *
 * ```
 * ion::renderer renderer = // ... //
 * int width, height;
 * SDL_GetRendererOutputSize(renderer, &width, &height);
 * ```
 *
 * \warn implicit `SDL_Renderer *` should only be used with primitive SDL calls.
 *       In particular, creating an `SDL_Renderer *` from an `ion::renderer`
 *       static factory method such as `ion::renderer::with_default_driver` will
 *       result in undefined behavior.
 */
class renderer {
public:
    // delete unwanted implicit constructors
    renderer(renderer const &) = delete;
    renderer() = delete;

    // resource management
    renderer(renderer && temp);
    ~renderer();

    // static factories

    /**
     * \brief Create a renderer with a driver at a specific index.
     *
     * \param sdl_window the window to render to
     * \param index the driver index
     * \param flags used to initialize the renderer
     */
    static tl::expected<renderer, std::string>
    with_driver_at(SDL_Window * sdl_window, int index, std::uint32_t flags = 0);

    /**
     * \brief Create a renderer with the first available driver.
     *
     * \param sdl_window the window to render to
     * \param flags used to initialize the renderer
     */
    static tl::expected<renderer, std::string>
    with_default_driver(SDL_Window * sdl_window, std::uint32_t flags = 0);

    inline operator SDL_Renderer * () { return _renderer; }
private:
    renderer(SDL_Renderer * sdl_renderer);
    SDL_Renderer * _renderer;
};
}
