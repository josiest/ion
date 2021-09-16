#pragma once

#include "types/tile.hpp"
#include <string>
#include <SDL2/SDL_surface.h>

namespace tiles {

/**
 * Get the image path of a tile bitmap
 * 
 * \param tilename the type name of the tile
 * \param rotation the rotation of the image
 *
 * \return a relative path from the binary directory to the image to load
 */
std::string image_path(Name tilename, Rotation rotation);

/**
 * Load all the tile bitmaps into a resource manager
 * \return map of each Name-Rotation pair to the respective surface
 */
tilemap load_map();

/**
 * Get the SDL_Surface associated with a name-rotation pair
 *
 * \param name the tile name of the surface to retrieve
 * \param rotation the rotation of the surface to retrieve
 *
 * \return the underlying SDL_Surface pointer
 *
 * Assumes that all possible name-rotation pairs exist as keys in tiles.
 */
SDL_Surface * get_surface(tilemap const & tiles, Name name, Rotation rotation);

}
