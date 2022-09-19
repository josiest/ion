#pragma once

// frameworks
#include <SDL.h>
#include <raisin/raisin.hpp>

// data types
#include <cstdint>
#include <string>

// resource handles
#include <tl/expected.hpp>

// algorithms

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

    /**
     * \brief create an SDL context from a config file
     *
     * \param config_path the path to the config file
     * \pram invalid_names a place to write invalid flag names to
     *
     * All subsystem flags must found under a "system" table
     * Parameters for the system
     *
     *  [system]
     *  subsystems = [<string>...]
     *
     * Acceptable flag names from a config file:
     *
     *  timer           - SDL_INIT_TIMER
     *  audio           - SDL_INIT_AUDIO
     *  video           - SDL_INIT_VIDEO
     *  joystick        - SDL_INIT_JOYSTICK
     *  game-controller - SDL_INIT_GAMECONTROLLER
     *  events          - SDL_INIT_EVENTS
     *  everything      - SDL_INIT_EVERYTHING
     *
     * example:
     *
     *  [system]
     *  subsystems = ["video", "audio"]
     */
    template<std::weakly_incrementable name_writer>
    static tl::expected<system, std::string>
    from_config(std::string const & path, name_writer invalid_names);
private:
    inline system() {}
    bool moved = false;
};

//
// Template Implementation
//

template<std::weakly_incrementable name_writer>
tl::expected<system, std::string>
system::from_config(std::string const & path, name_writer invalid_names)
{
    auto system_result = raisin::init_sdl_from_config(path, invalid_names);
    if (not system_result) {
        return tl::unexpected(system_result.error());
    }
    return system{};
}

}
