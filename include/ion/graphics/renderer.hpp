#pragma once

// frameworks
#include <SDL.h>
#include <raisin/raisin.hpp>

// data types
#include <cstdint>
#include <string>

// resource handles
#include <tl/expected.hpp>

// algorithms
#include <iterator>

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

    /**
     * \brief Create a renderer from a config file.
     *
     * \param sdl_window the window to render to
     * \param config_path the path of the config file
     * \param invalid_names a place to write invalid render flag names
     */
    template<std::weakly_incrementable name_writer>
    static tl::expected<renderer, std::string>
    from_config(SDL_Window * sdl_window, std::string const & path,
                name_writer invalid_names);

    inline operator SDL_Renderer * () const { return _renderer; }
private:
    renderer(SDL_Renderer * sdl_renderer);
    SDL_Renderer * _renderer;
};

//
// Template Implementation
//

template<std::weakly_incrementable name_writer>
tl::expected<renderer, std::string>
renderer::from_config(SDL_Window * window, std::string const & path,
                      name_writer invalid_names)
{
    auto renderer_result = raisin::make_renderer_from_config(
            path, window, invalid_names);
    if (not renderer_result) {
        return tl::unexpected(renderer_result.error());
    }
    return renderer{std::move(renderer_result).value()};
}
}
