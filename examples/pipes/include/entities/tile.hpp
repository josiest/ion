#pragma once

#include <ion/graphics/surface.hpp>
#include <SDL2/SDL_surface.h>
#include <entt.hpp>

#include <ostream>
#include <string>

#include <unordered_set>
#include <unordered_map>

namespace tiles {

enum class Name { End, Bar, Bend, Fork };
enum class Rotation { Right, Up, Left, Down };

std::ostream & operator<<(std::ostream & os, Name tilename);
std::ostream & operator<<(std::ostream & os, Rotation rot);
std::string image_path(Name tilename, Rotation rotation);

inline std::unordered_set<Name> names = {
    Name::End, Name::Bar, Name::Bend, Name::Fork
};
inline std::unordered_set<Rotation> rotations = {
    Rotation::Right, Rotation::Up, Rotation::Left, Rotation::Down
};

class map {
public:
    map();
    SDL_Surface * sdl_surface(Name tilename, Rotation rotation);
    SDL_Surface const * sdl_surface(Name tilename, Rotation rotation) const;
    inline void clear() { _images.clear(); }
private:
    std::unordered_map<Name, std::unordered_map<Rotation, ion::surface>> _images;
};

// registry functions

entt::entity make(entt::registry & registry,
                  Name tilename, Rotation rotation, int x, int y);

void move(entt::registry & registry, entt::entity tile, int x, int y);
void rotate(entt::registry & registry, entt::entity tile, Rotation rotation);

}
