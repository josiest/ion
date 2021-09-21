#pragma once

#include <SDL.h>

#include <memory>
#include <string>

#include <stdexcept>
#include <sstream>

namespace ion {

/**
 * Initialize the video subsystem of SDL if it hasn't been initialized
 * \throw std::runtime_error if video couldn't initialize for any reason
 *
 * This is amlost the same as SDL_InitSubSystem(SDL_INIT_VIDEO), but it doesn't
 * do anything if the subsystem was already intialized, and it throws an error
 * when initialization fails.
 */
void init_video();

/**
 * Initialize the image subsystem of SDL if it hasn't been initialized
 * \param flags the SDL_image flags to initialize with
 * \throw std::runtime_error if SDL_image couldn't initialize for any reason
 *
 * This is essentially the same as IMG_init(flags), but it throws an error when
 * initialization fails.
 */
void init_image(int flags);

/** A pointer type */
template<typename ptr_t, typename underlying_t>
concept pointer = requires(ptr_t ptr) {
    { *ptr } -> std::same_as<underlying_t>;
};

/** A type alias for a unique pointer to an SDL_Surface */
using unique_surface = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>;
/** A type alias for a shared pointer to an SDL_Surface */
using shared_surface = std::shared_ptr<SDL_Surface>;

/** A type alias for a unique pointer to an SDL_Texture */
using unique_texture = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;
/** A type alias for a shared pointer to an SDL_Texture */
using shared_texture = std::shared_ptr<SDL_Texture>;

/**
 * Load an image as an SDL_Surface 
 *
 * \param path the path of the image. Assumes whatever subsystems needed
 *                 to load the image have been initialized.
 *
 * \return the SDL_Surface pointer with the bitmap data
 * \throw std::runtime_error if the image path couldn't be loaded
 */
SDL_Surface * load_surface(std::string const & path);

}
