#pragma once
#include <ion/render/bitmap.hpp>
#include <SDL2/SDL_surface.h>

#include <ostream>
#include <string>

#include <unordered_set>
#include <unordered_map>

namespace tiles {

enum class TileName { End, Bar, Bend, Fork };
enum class Rotation { Right, Up, Left, Down };

std::ostream & operator<<(std::ostream & os, TileName tile);
std::ostream & operator<<(std::ostream & os, Rotation rot);

inline std::unordered_set<TileName> names = {
    TileName::End, TileName::Bar, TileName::Bend, TileName::Fork
};
inline std::unordered_set<Rotation> rotations = {
    Rotation::Right, Rotation::Up, Rotation::Left, Rotation::Down
};

std::string image_path(TileName tile, Rotation rot);

class TileMap {
public:
    TileMap();
    SDL_Surface * sdl_surface(TileName tile, Rotation rot);
    SDL_Surface const * sdl_surface(TileName tile, Rotation rot) const;
private:
    std::unordered_map<TileName, std::unordered_map<Rotation, ion::Bitmap>> _images;
};

}
