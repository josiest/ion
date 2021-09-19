#pragma once

#include <cstdint>
#include <string>

namespace ion {

class sdl_context {
public:
    // delete any unwanted implicit constructors
    sdl_context(sdl_context const &) = delete;

    /**
     * Create an SDL resource handler with only video initialized
     */
    sdl_context() noexcept;

    /**
     * Create an SDL resource handler
     *
     * \param flags the flags to initialize SDL
     *
     * If SDL couldn't intitalize, sdl_context::good() will return false. If SDL
     * did initialize, SDL_Quit will be called when the lifetime of this object
     * ends.
     */
    sdl_context(std::uint32_t flags) noexcept;
    ~sdl_context();

    /**
     * Determine if sdl was initialized
     */
    bool good() const noexcept;

    /**
     * Get the error message for why SDL couldn't initialize
     */
    std::string error() const noexcept;
private:
    std::string _error;
};
}
