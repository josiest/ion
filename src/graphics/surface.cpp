#include "ion/graphics/surface.hpp"
#include <SDL.h>
#include <stdexcept>
#include <sstream>

namespace ion {

surface::surface(SDL_Surface * surface) : _surface{surface} {}
surface::surface(surface const & other)

    // create the surface with the same properties as the given surface
    : _surface{SDL_CreateRGBSurfaceWithFormat(
          0, other.sdl_surface()->w, other.sdl_surface()->h,
          other.sdl_surface()->format->BitsPerPixel,
          other.sdl_surface()->format->format
      )}
{
    // surface creation may have failed
    if (!_surface) {
        std::stringstream message;
        message << "surface memory couldn't be allocated! SDL_Error: "
                << SDL_GetError();
        throw std::runtime_error{message.str()};
    }
    // copy from the given surface
    auto source = const_cast<SDL_Surface *>(other.sdl_surface());
    bool const copy_successful =
        // any changes to src through BlitSurface are _not_ documented, so to
        // the end user, we can assume that it's behaviorally constant
        SDL_BlitSurface(source, nullptr, _surface, nullptr) == 0;

    // if failure clean up and throw error
    if (!copy_successful) {
        std::stringstream message;
        message << "surface wasn't able to copy successfully! SDL_Error: "
                << SDL_GetError();

        // clean up
        SDL_FreeSurface(_surface);
        _surface = nullptr;
        throw std::runtime_error{message.str()};
    }
}

surface::~surface()
{
    if (_surface) {
        SDL_FreeSurface(_surface);
    }
    _surface = nullptr;
}
}
