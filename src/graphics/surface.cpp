#include "ion/graphics/surface.hpp"
#include "ion/error.hpp"

#include <SDL.h>
#include <string>

namespace ion {

surface::surface(SDL_Surface * surface) noexcept
    : _surface{surface}
{
    // failure if given null
    if (not surface) {
        set_error("surface initialize with null");
    }
}

surface::surface(std::string const & path) noexcept
    : _surface{SDL_LoadBMP(path.c_str())}
{
    // use SDL's error message if LoadBMP failed
    if (not _surface) {
        set_error(SDL_GetError());
    }
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
