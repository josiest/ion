#include "systems/resource.hpp"

// filenames
#include <string>
#include <sstream>

// type declarations
#include "types/tile.hpp"
#include <SDL2/SDL_surface.h>

// functionality
#include <ion/graphics/image.hpp>
#include <utility>

namespace tiles {

std::string image_path(Name tilename, Rotation rotation)
{
    std::stringstream path;
    path << "../resources/" << tilename << "-" << rotation << ".bmp";
    return path.str();
}

tilemap load_map()
{
    tilemap tiles;
    // full cartesian product of tile names and rotations
    for (Name tilename : names) {
        for (Rotation rotation : rotations) {

            // generate the path-name to the tilename-rotation pair
            auto const path = image_path(tilename, rotation);

            // load the surface as a unique pointer into the map
            tiles.try_emplace({tilename, rotation},
                              ion::load_surface(path), &SDL_FreeSurface);
        }
    }
    return tiles;
}

SDL_Surface * get_surface(tilemap const & tiles, Name name, Rotation rotation)
{
    return tiles.at({name, rotation}).get();
}

}
