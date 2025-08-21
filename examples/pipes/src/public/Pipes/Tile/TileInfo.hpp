#pragma once
#include <entt/meta/factory.hpp>

#include <array>
#include <random>

namespace Pipes::TileInfo {

// the possible tilenames
enum class Name { End=0, Bar, Bend, Fork };
constexpr std::array names
{
    Name::End, Name::Bar, Name::Bend, Name::Fork
};

// the possible tile rotations
enum class Rotation { Right=0, Up, Left, Down };
constexpr std::array rotations
{
    Rotation::Right, Rotation::Up, Rotation::Left, Rotation::Down
};

constexpr Rotation& operator++(Rotation& rotation);
constexpr Rotation& operator--(Rotation& rotation);
constexpr Rotation operator++(Rotation& rotation, int);
constexpr Rotation operator--(Rotation& rotation, int);

/** Generate a random tilename */
template<typename RandomEngine> // requires RandomNumberEngine<RandomEngine>
Name random_name(RandomEngine & rng)
{
    std::uniform_int_distribution<> index(0, names.size()-1);
    auto choice = names.begin();
    std::advance(choice, index(rng));
    return *choice;
}

/** Generate a random rotation */
template<typename RandomEngine> // requires RandomNumberEngine<RandomEngine>
Rotation random_rotation(RandomEngine & rng)
{
    std::uniform_int_distribution<> index(0, rotations.size()-1);
    auto choice = rotations.begin();
    std::advance(choice, index(rng));
    return *choice;
}
}

/** Print a tile name */
std::ostream & operator<<(std::ostream & os, Pipes::TileInfo::Name tilename);
/** Print a tile rotation */
std::ostream & operator<<(std::ostream & os, Pipes::TileInfo::Rotation rot);


constexpr Pipes::TileInfo::Rotation & Pipes::TileInfo::operator++(Rotation & rotation)
{
    switch (rotation)
    {
        case Rotation::Right:
            rotation = Rotation::Up;
            break;
        case Rotation::Up:
            rotation = Rotation::Left;
            break;
        case Rotation::Left:
            rotation = Rotation::Down;
            break;
        case Rotation::Down:
            rotation = Rotation::Right;
            break;
        default:
            break;
    }
    return rotation;
}

constexpr Pipes::TileInfo::Rotation & Pipes::TileInfo::operator--(Rotation & rotation)
{
    switch (rotation)
    {
        case Rotation::Right:
            rotation = Rotation::Down;
            break;
        case Rotation::Up:
            rotation = Rotation::Right;
            break;
        case Rotation::Left:
            rotation = Rotation::Up;
            break;
        case Rotation::Down:
            rotation = Rotation::Left;
            break;
        default:
            break;
    }
    return rotation;
}

constexpr Pipes::TileInfo::Rotation Pipes::TileInfo::operator++(Rotation & rotation, int)
{
    const Rotation copy = rotation;
    ++rotation;
    return copy;
}

constexpr Pipes::TileInfo::Rotation Pipes::TileInfo::operator--(Rotation & rotation, int)
{
    const Rotation copy = rotation;
    --rotation;
    return copy;
}
