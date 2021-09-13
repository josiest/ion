#include "ion/graphics/image.hpp"
#include <SDL2/SDL.h>
#include <string>
#include <stdexcept>
#include <sstream>

namespace ion {

SDL_Surface * load_bitmap_image(std::string const & path)
{
    // failure if SDL can't init video
    if (SDL_WasInit(SDL_INIT_VIDEO) == 0
            && SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {

        std::stringstream message;
        message << "Video couldn't initialize! SDL_Error: " << SDL_GetError();
        throw std::runtime_error{message.str()};
    }

    // failure if image couldn't be loaded
    SDL_Surface * surface = SDL_LoadBMP(path.c_str());
    if (!surface) {
        std::stringstream message;
        message << "Couldn't load bitmap! SDL_Error: " << SDL_GetError();
        throw std::runtime_error{message.str()};
    }
    return surface;
}
}
