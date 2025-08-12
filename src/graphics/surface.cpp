#include "ion/graphics/surface.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_rwops.h>

#include <string_view>

#define SDL_CLOSE_FILESTREAM 1

namespace ion {

surface::surface(SDL_Surface * surface) noexcept
    : _surface{surface}
{
    // failure if given null
    if (not surface) {
        set_error("surface initialized with null");
    }
}

surface::surface(surface && temp) noexcept
    : _surface{temp._surface}
{
    temp._surface = nullptr;
    set_error(temp.get_error());
    temp.set_error("surface moved to another object");
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

surface surface::load_bitmap(const std::string_view & path) noexcept
{
    surface image{ SDL_LoadBMP(path.begin()) };
    if (not image) { image.set_error(SDL_GetError()); }

    return image;
}

}
