#include "ion/graphics/surface.hpp"

#include <SDL.h>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace ion {

surface::surface(SDL_Surface * surface) noexcept
    : _surface{surface}
{
    // failure if given null
    if (not surface) {
        _error = "surface initialized with null";
    }
}

surface::surface(fs::path const & path) noexcept
    : _surface{SDL_LoadBMP(path.c_str())}
{
    // use SDL's error message if LoadBMP failed
    if (not _surface) {
        _error = SDL_GetError();
    }
}

surface::surface(surface && temp) noexcept
    : _surface{temp._surface}, _error{std::move(temp._error)}
{
    temp._surface = nullptr;
    temp._error = "surface moved to another object";
}

surface::~surface()
{
    // it's possible that the surface may be null if initialization failed
    // so only free the surface if it's not null
    if (_surface) {
        SDL_FreeSurface(_surface);
    }
    _surface = nullptr;
}
}
