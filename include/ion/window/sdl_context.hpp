#pragma once

#include "ion/isotope.hpp"
#include <cstdint>
#include <string>

namespace ion {

/** A resource handler for SDL-initialization and termination */
class sdl_context : public isotope {
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
};
}
