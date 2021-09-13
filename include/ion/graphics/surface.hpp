#pragma once
#include <SDL2/SDL.h>

namespace ion {

/**
 * A resource handler for a surface image
 */
class surface {
public:
    surface() = delete;

    /**
     * Create a surface from an SDL_Surface
     *
     * \param surface the surface of the surface
     * \param optimized_to the optional renderer to optimize the surface to
     *
     * \note If a renderer is specified to optimize the surface to, the original
     *       surface pointer is invalidated and its memory freed, even when the
     *       constructor is unsuccesful. Otherwise, it will be used as its
     *       underlying SDL_Surface.
     */
    surface(SDL_Surface * surface);

    /**
     * Create a surface from another surface
     */
    surface(surface const & other);
    ~surface();

    /**
     * Get the underlying SDL_Surface of the surface
     */
    inline SDL_Surface * sdl_surface() const { return _surface; }
private:
    SDL_Surface * _surface;
};
}
