#include "ion/graphics/surface.hpp"
#include "ion/isotope.hpp"

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

surface surface::load_bitmap(fs::path const & path) noexcept
{
    surface image{SDL_LoadBMP(path.c_str())};

    // use SDL's error message if LoadBMP failed
    if (not image) {
        image.set_error(SDL_GetError());
    }

    return image;
}

}
