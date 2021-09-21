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

    /** Determine if sdl was initialized properly */
    inline bool good() const noexcept { return _error.empty(); }

    /** Determine if sdl encountered a non-recoverable error */
    inline bool bad() const noexcept { return not _error.empty(); }

    /** Why SDL couldn't initialize */
    inline std::string error() const noexcept { return _error; }
private:
    std::string _error;
};
}
