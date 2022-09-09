#pragma once

// frameworks
#include <SDL.h>

// data types
#include <cstdint>
#include <string>

// resource handles
#include <tl/expected.hpp>

namespace ion {

/** A resource handler for SDL-initialization and termination.
 *
 * The ion::system is a little bit odd, since it's only meant to initialize and
 * tear down SDL subsystems. You won't ever use the object instance. It's
 * certainly possible to use ion without creating an sdl_context if you just
 * call the respective *Init and *Quit functions, but the ion::system does this
 * automatically by taking advantage of the object's lifetime.
 */
class system {
public:
    // delete unwanted implicit constructors
    system(system const &) = delete;
    // resource management
    inline system(system && sys) { moved = true; }
    ~system();

    // static factories

    /**
     * \brief create an SDL context with explicitly defined susbystems
     *
     * \param flags the subsystem flags to initialize
     */
    static tl::expected<system, std::string>
    with_subsystems(std::uint32_t flags);

    /**
     * \brief create an SDL context with video and audio initialized
     *
     * \param flags additional subsystem flags to initialize SDL with
     */
    static tl::expected<system, std::string>
    with_defaults(std::uint32_t flags = 0);
private:
    inline system() {}
    bool moved = false;
};
}
