#pragma once
#include <SDL2/SDL.h>

namespace ion {

/**
 * A resource handler for a bitmap image
 */
class Bitmap {
public:
    Bitmap() = delete;

    /**
     * Create a bitmap from an SDL_Surface
     *
     * \param surface the surface of the bitmap
     * \param optimized_to the optional renderer to optimize the surface to
     *
     * \note If a renderer is specified to optimize the bitmap to, the original
     *       surface pointer is invalidated and its memory freed, even when the
     *       constructor is unsuccesful. Otherwise, it will be used as its
     *       underlying SDL_Surface.
     */
    Bitmap(SDL_Surface * surface);

    /**
     * Create a bitmap from another bitmap
     */
    Bitmap(Bitmap & bitmap);
    ~Bitmap();

    /**
     * Get the underlying SDL_Surface of the bitmap
     */
    inline SDL_Surface * sdl_surface() { return _surface; }
private:
    SDL_Surface * _surface;
};
}
