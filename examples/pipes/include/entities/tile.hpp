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

inline void increment_rotation(Rotation & rotation)
{
    switch (rotation) {
    case Rotation::Right: rotation = Rotation::Up; break;
    case Rotation::Up: rotation = Rotation::Left; break;
    case Rotation::Left: rotation = Rotation::Down; break;
    case Rotation::Down: rotation = Rotation::Right; break;
    default: break;
    }
}

inline void decrement_rotation(Rotation & rotation)
{
    switch (rotation) {
    case Rotation::Right: rotation = Rotation::Down; break;
    case Rotation::Up: rotation = Rotation::Right; break;
    case Rotation::Left: rotation = Rotation::Up; break;
    case Rotation::Down: rotation = Rotation::Left; break;
    default: break;
    }
}

std::ostream & operator<<(std::ostream & os, Name tilename);
std::ostream & operator<<(std::ostream & os, Rotation rot);
std::string image_path(Name tilename, Rotation rotation);

inline std::unordered_set<Name> names = {
    Name::End, Name::Bar, Name::Bend, Name::Fork
};
inline std::unordered_set<Rotation> rotations = {
    Rotation::Right, Rotation::Up, Rotation::Left, Rotation::Down
};

template<typename engine_t>
Name random_name(engine_t rng)
{
    std::uniform_int_distribution<> index(0, names.size()-1);
    auto choice = names.begin();
    std::advance(choice, index(rng));
    return *choice;
}

template<typename engine_t>
Rotation random_rotation(engine_t rng)
{
    std::uniform_int_distribution<> index(0, rotations.size()-1);
    auto choice = rotations.begin();
    std::advance(choice, index(rng));
    return *choice;
}

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

entt::entity make(entt::registry & entities,
                  Name tilename, Rotation rotation, int x, int y);

inline entt::entity make(entt::registry & entities,
                         Name tilename, Rotation rotation, SDL_Point const & p)
{
    return make(entities, tilename, rotation, p.x, p.y);
}

void move(entt::registry & registry, entt::entity tile, int x, int y);
void rotate(entt::registry & registry, entt::entity tile, Rotation rotation);

}
