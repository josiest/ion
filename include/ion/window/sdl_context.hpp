#pragma once

#include "ion/isotope.hpp"
#include <SDL.h>

// datatypes
#include <cstdint>
#include <string>

// data structures
#include <vector>

namespace ion {

/** A resource handler for SDL-initialization and termination.
 *
 * The sdl_context is a little bit odd, since it's only meant to initialize and
 * tear down SDL subsystems. You won't ever use the object created, other than
 * to initialize subsystems like SDL_Image or SDL_Mixer. It's certainly possible
 * to use ion without creating an sdl_context if you just call the respective
 * *Init and *Quit functions, but the sdl_context does this automatically by
 * taking advantage of the object lifetime.
 */
class sdl_context : public isotope {
public:
    // delete any unwanted implicit constructors
    sdl_context(sdl_context const &) = delete;

    /**
     * Create an SDL resource handler
     *
     * \param flags the flags to initialize SDL
     *
     * If SDL was initialized, SDL_Quit will be called when the lifetime of
     * this object ends.
     */
    sdl_context(std::uint32_t flags = SDL_INIT_VIDEO) noexcept;
    ~sdl_context();

    /**
     * Initialize ttf
     */
    void init_ttf() noexcept;
private:
    std::vector<void(*)(void)> subsystem_destructors;

    void cleanup_everything();
};
}
