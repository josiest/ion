#pragma once

// frameworks
#include <SDL.h>

// data types
#include <string>
#include <cstdint>

// resource handles
#include <tl/expected.hpp>

namespace ion {

/** A resource handle for an SDL_Renderer. */
class renderer {
public:
    // delete unwanted implicit constructors
    renderer() = delete;
    renderer(renderer const &) = delete;

    // resource management
    renderer(renderer && temp);
    ~renderer();

    // static factories

    /**
     * \brief Create a renderer from a driver at a given index
     *
     * \param win the window to link to
     * \param index of the driver
     * \param flags to initialize the renderer
     */
    static tl::expected<renderer, std::string>
    with_driver_at(SDL_Window * win, int index, std::uint32_t flags = 0);

    /**
     * \brief Create a renderer from the first available river
     *
     * \param win the window to link to
     * \param flags to initialize the renderer
     */
    static tl::expected<renderer, std::string>
    with_default_driver(SDL_Window * win, std::uint32_t flags = 0);

    /** The underlying SDL_Renderer */
    inline operator SDL_Renderer *() { return _renderer; }
private:
    SDL_Renderer * _renderer;

    renderer(SDL_Renderer * sdl_renderer);
};
}
