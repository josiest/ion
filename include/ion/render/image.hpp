#pragma once
#include <SDL2/SDL.h>
#include <string>

namespace ion {

/**
 * Load a bitmap image
 *
 * \param path the path of the bitmap image
 * \return the SDL_Surface pointer with the bitmap data
 * \throw std::runtime_error if SDL couldn't intialize video, or if the image
 *        path couldn't be loaded
 */
SDL_Surface * load_bitmap_image(std::string const & path);
}
