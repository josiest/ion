#pragma once
#include "ion/isotope.hpp"

#include <SDL2/SDL.h>

#include <string>
#include <string_view>

namespace ion {

/**
 * A resource handler for a surface image
 */
class surface : public isotope {
public:
    // delete unwanted implicit constructors
    surface() = delete;
    surface(surface const &) = delete;

    /**
     * Create a surface from an SDL_Surface
     * \param surface the underlying SDL_Surface
     */
    surface(SDL_Surface * surface) noexcept;

    /**
     * Create a surface from another temporary surface
     * \param temp the temporary surface
     */
    surface(surface && temp) noexcept;
    ~surface();

    /** Cast to the underlying SDL_Surface */
    inline operator SDL_Surface *() noexcept { return _surface; }

    /**
     * Create a surface by loading a bitmap image
     * \param path the path of the bitmap image
     */
    static surface load_bitmap(const std::string_view & path) noexcept;
private:
    SDL_Surface * _surface;
};
}
