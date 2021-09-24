#pragma once
#include <SDL.h>
#include <string>

namespace ion {

/**
 * A resource handler for a surface image
 */
class surface {
public:
    // delete unwanted implicit constructors
    surface() = delete;
    surface(surface const &) = delete;

    /**
     * Create a surface from an SDL_Surface
     *
     * \param surface the underlying SDL_Surface
     */
    surface(SDL_Surface * surface) noexcept;

    /**
     * Create a surface by loading a bitmap image
     *
     * \param path the path of the bitmap image
     */
    surface(std::string const & path) noexcept;
    ~surface();

    /** Cast to the underlying SDL_Surface */
    inline operator SDL_Surface *() noexcept { return _surface; }

    /** Determine if the surface is not okay to use */
    inline bool operator!() const noexcept { return nullptr == _surface; }

    /** The error if the surface is not okay to use */
    inline std::string const & error() const noexcept { return _error; }
private:
    SDL_Surface * _surface;
    std::string _error;
};
}
