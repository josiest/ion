#include "ion/render/bitmap.hpp"
#include <SDL2/SDL.h>
#include <stdexcept>

namespace ion {

Bitmap::Bitmap(SDL_Surface * surface) : _surface{surface} {}
Bitmap::Bitmap(Bitmap & bitmap)

    // create the surface with the same properties as the given bitmap
    : _surface{SDL_CreateRGBSurfaceWithFormat(
          0, bitmap.sdl_surface()->w, bitmap.sdl_surface()->h,
          bitmap.sdl_surface()->format->BitsPerPixel,
          bitmap.sdl_surface()->format->format
      )}
{
    // surface creation may have failed
    if (!_surface) {
        throw std::runtime_error{SDL_GetError()};
    }
    // copy from the given bitmap
    bool const copy_successful =
        SDL_BlitSurface(bitmap.sdl_surface(), nullptr, _surface, nullptr) != 0;

    // if failure clean up and throw error
    if (!copy_successful) {
        SDL_FreeSurface(_surface);
        _surface = nullptr;
        throw std::runtime_error{SDL_GetError()};
    }
}

Bitmap::~Bitmap()
{
    if (_surface) {
        SDL_FreeSurface(_surface);
    }
    _surface = nullptr;
}
}
