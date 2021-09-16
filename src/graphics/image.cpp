// header declarations
#include "ion/graphics/image.hpp"

// types used
#include <SDL2/SDL_surface.h>
#include <string>

// functions used
#include <SDL2/SDL_image.h>

// exceptions and messages
#include <stdexcept>
#include <sstream>

namespace ion {

void init_video()
{
    // first check if the video subystem was initialized
    if (SDL_WasInit(SDL_INIT_VIDEO) == 0
            // if it wasn't try to initialize it
            && SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {

        // failure if video subystem couldn't initialize
        std::stringstream message;
        message << "Video couldn't initialize! SDL_Error: " << SDL_GetError();
        throw std::runtime_error{message.str()};
    }
}

void init_image(int flags)
{
    // try to initialize SDL_image: returned flags should have the same mask
    if (IMG_Init(flags) & flags != flags) {

        // failure if the flags passed weren't initialized
        std::stringstream message;
        message << "SDL_image Couldn't initialize image! IMG_Error: "
                << IMG_GetError();
        throw std::runtime_error{message.str()};
    }
}

SDL_Surface * load_surface(std::string const & path)
{
    // try to load the image as a surface
    SDL_Surface * surface = IMG_Load(path.c_str());

    // failure if image couldn't be loaded
    if (!surface) {
        // get the error from sdl first
        std::string sdl_error = SDL_GetError();

        // if no error then the error was with SDL_image
        if (sdl_error.empty()) {
            sdl_error = IMG_GetError();
        }
        std::stringstream message;
        message << "Couldn't load bitmap! Error: " << sdl_error;
        throw std::runtime_error{message.str()};
    }
    return surface;
}
}
