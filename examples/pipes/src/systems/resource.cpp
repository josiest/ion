#include "systems/resource.hpp"

// filenames
#include <string>
#include <sstream>

// type declarations
#include "types/tile.hpp"
#include <SDL2/SDL_surface.h>

// functionality
#include <ion/graphics/surface.hpp>
#include <utility>

#include "error.hpp"

namespace tiles {

std::string image_path(Name tilename, Rotation rotation)
{
    std::stringstream path;
    path << "../resources/" << tilename << "-" << rotation << ".bmp";
    return path.str();
}

tilemap load_map()
{
    // keep track of the the files that fail to load
    std::string error;
    std::string sep = "";

    // initialize the tilemap as empty, add tiles as we go
    tilemap tiles;

    // full cartesian product of tile names and rotations
    for (Name tilename : names) {
        for (Rotation rotation : rotations) {

            // generate the path-name to the tilename-rotation pair
            auto const path = image_path(tilename, rotation);

            // load the surface as a unique pointer into the map
            tiles.emplace(std::make_pair(tilename, rotation),
                          std::make_unique<ion::surface>(path));

            // save the error message if loading failed
            auto const & surface = *tiles.at({tilename, rotation});
            if (not surface) {
                error = sep + surface.error();
                sep = "\n";
            }
        }
    }
    // set the game error message
    set_error(error);
    return tiles;
}

SDL_Surface * get_surface(tilemap const & tiles, Name name, Rotation rotation)
{
    return *tiles.at({name, rotation});
}

}
