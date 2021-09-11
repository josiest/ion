#include "ion/render/image.hpp"
#include <SDL2/SDL.h>
#include <string>
#include <stdexcept>

namespace ion {

SDL_Surface * load_bitmap_image(std::string const & path)
{
    // failure if SDL can't init video
    if (!SDL_WasInit(SDL_INIT_VIDEO) && SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error{SDL_GetError()};
    }

    // failure if image couldn't be loaded
    SDL_Surface * surface = SDL_LoadBMP(path.c_str());
    if (!surface) {
        throw std::runtime_error{SDL_GetError()};
    }
    return surface;
}
}
