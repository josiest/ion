#pragma once
#include <SDL.h>
#include <string>
#include <filesystem>

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
     * \param surface the underlying SDL_Surface
     */
    surface(SDL_Surface * surface) noexcept;

    /**
     * Create a surface by loading a bitmap image
     * \param path the path of the bitmap image
     */
    explicit surface(std::filesystem::path const & path) noexcept;

    /**
     * Create a surface from another temporary surface
     * \param temp the temporary surface
     */
    surface(surface && temp) noexcept;
    ~surface();

    /** Cast to the underlying SDL_Surface */
    inline operator SDL_Surface *() noexcept { return _surface; }

    /** Determine if the surface is not okay to use */
    inline bool operator!() const noexcept { return not _surface; }

    /** The error if the surface is not okay to use */
    inline std::string const & error() const noexcept { return _error; }
private:
    SDL_Surface * _surface;
    std::string _error;
};
}
