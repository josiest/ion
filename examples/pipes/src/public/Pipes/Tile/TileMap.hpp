#pragma once
#include "Pipes/Tile/TileInfo.hpp"
#include <ion/graphics/surface.hpp>
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
        // name and rotation each take up two bits, so we can hash them together as a 4-bit number
        return (static_cast<size_t>(tile.name) << 2) | static_cast<size_t>(tile.rotation);
    }
};

namespace Pipes
{
class TileMap
{
public:
    TileMap(const ion::asset_loader& asset_loader, std::string_view images_path);
    SDL_Surface * image_for(TileInfo::Name name, TileInfo::Rotation rotation) const;
private:
    TileMap() = default;
    std::unordered_map<TileID, ion::surface> tiles;
};
}
