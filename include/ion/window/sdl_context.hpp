#pragma once

#include <cstdint>
#include <string>

namespace ion {

/** A resource handler for SDL-initialization and termination */
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
     * If SDL was initialized, SDL_Quit will be called when the lifetime of
     * this object ends.
     */
    sdl_context(std::uint32_t flags) noexcept;
    ~sdl_context();

    /** Determine if sdl is not okay to use */
    inline bool operator!() const noexcept { return not _error.empty(); }

    /** The error if sdl is not okay to use */
    inline std::string error() const noexcept { return _error; }
private:
    std::string _error;
};
}
