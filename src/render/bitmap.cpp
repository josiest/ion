#include "ion/render/bitmap.hpp"
#include <SDL2/SDL.h>
#include <stdexcept>
#include <sstream>

namespace ion {

Bitmap::Bitmap(SDL_Surface * surface) : _surface{surface} {}
Bitmap::Bitmap(Bitmap const & bitmap)

    // create the surface with the same properties as the given bitmap
    : _surface{SDL_CreateRGBSurfaceWithFormat(
          0, bitmap.sdl_surface()->w, bitmap.sdl_surface()->h,
          bitmap.sdl_surface()->format->BitsPerPixel,
          bitmap.sdl_surface()->format->format
      )}
{
    // surface creation may have failed
    if (!_surface) {
        std::stringstream message;
        message << "Bitmap memory couldn't be allocated! SDL_Error: "
                << SDL_GetError();
        throw std::runtime_error{message.str()};
    }
    // copy from the given bitmap
    bool const copy_successful =
        // any changes to src through BlitSurface are _not_ documented, so to
        // the end user, we can assume that it's behaviorally constant
        SDL_BlitSurface(const_cast<SDL_Surface *>(bitmap.sdl_surface()), nullptr,
                        _surface, nullptr) == 0;

    // if failure clean up and throw error
    if (!copy_successful) {
        std::stringstream message;
        message << "Bitmap wasn't able to copy successfully! SDL_Error: "
                << SDL_GetError();

        // clean up
        SDL_FreeSurface(_surface);
        _surface = nullptr;
        throw std::runtime_error{message.str()};
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
