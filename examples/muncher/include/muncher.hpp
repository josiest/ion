#pragma once

// game framework
#include "entities/munchable.hpp"

// frameworks
#include <ion/ion.hpp>
#include <SDL2/SDL.h>

// data types
#include <string>

// resource handles
#include <tl/expected.hpp>

class muncher{
public:
    // get rid of unwanted implicit constructors
    muncher(muncher const &) = delete;
    muncher() = delete;

    muncher(muncher && temp) = default;

    /**
     * \brief Create a muncher game instance from a config file.
     *
     * \param config_path the path of the config file.
     */
    static tl::expected<muncher, std::string>
    from_config(std::string const & config_path);

    /** Get a reference to the game window */
    inline SDL_Window * window() const { return _window; }

    /** Get a reference to the game renderer */
    inline SDL_Renderer * renderer() const { return _renderer; }
private:
    muncher(ion::system && system, ion::window && window,
            ion::renderer && renderer, munchable_spawner && munchables);

    ion::system _system;
    ion::window _window;
    ion::renderer _renderer;

    munchable_spawner _munchables;
};
