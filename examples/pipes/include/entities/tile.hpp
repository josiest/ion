#pragma once

#include <ion/graphics/surface.hpp>
#include <SDL2/SDL_surface.h>
#include <entt.hpp>

#include <ostream>
#include <string>

#include <unordered_set>
#include <unordered_map>

#include <random>
#include <iterator>

namespace tiles {

enum class Name { End, Bar, Bend, Fork };
enum class Rotation { Right, Up, Left, Down };

inline std::unordered_set<Name> names = {
    Name::End, Name::Bar, Name::Bend, Name::Fork
};
inline std::unordered_set<Rotation> rotations = {
    Rotation::Right, Rotation::Up, Rotation::Left, Rotation::Down
};

std::ostream & operator<<(std::ostream & os, Name tilename);
std::ostream & operator<<(std::ostream & os, Rotation rot);
std::string image_path(Name tilename, Rotation rotation);

class map {
public:
    map();
    SDL_Surface * sdl_surface(Name tilename, Rotation rotation);
    inline void clear() { _images.clear(); }
private:
    std::unordered_map<Name, std::unordered_map<Rotation, ion::surface>> _images;
};

entt::entity make(entt::registry & entities,
                  Name name, Rotation rotation, int x, int y);

inline entt::entity make(entt::registry & entities,
                         Name name, Rotation rotation, SDL_Point const & p)
{
    return make(entities, name, rotation, p.x, p.y);
}

entt::entity make_static(entt::registry & entities,
                         Name tilename, Rotation rotation, int x, int y);

}
