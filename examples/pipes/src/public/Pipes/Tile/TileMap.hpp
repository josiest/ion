#pragma once
#include "Pipes/Tile/TileInfo.hpp"
#include <string_view>

namespace ion
{
class surface;
class asset_loader;
}

struct SDL_Surface;

namespace Pipes
{
struct TileID
{
    TileInfo::Name name;
    TileInfo::Rotation rotation;
};

constexpr bool operator==(const TileID & lhs, const TileID & rhs)
{
    return lhs.name == rhs.name and lhs.rotation == rhs.rotation;
}
}

template<> struct std::hash<Pipes::TileID>
{
    constexpr size_t operator()(const Pipes::TileID & tile) const noexcept
    {
        constexpr hash<Pipes::TileInfo::Name> hash_name;
        constexpr hash<Pipes::TileInfo::Rotation> hash_rotation;
        return hash_name(tile.name) ^ hash_rotation(tile.rotation);
    }
};

namespace Pipes
{
class TileMap
{
public:
    static TileMap load_all(const ion::asset_loader& asset_loader, std::string_view images_path);
    SDL_Surface * image_for(TileInfo::Name name, TileInfo::Rotation rotation);
private:
    TileMap() = default;
    std::unordered_map<TileID, ion::surface> tiles;
};
}
