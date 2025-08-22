#pragma once

#include "Pipes/Tile/TileInfo.hpp"
#include "Pipes/Tile/TileMap.hpp"

#include "components.hpp"
#include "systems/point.hpp"
#include "systems/tile.hpp"

#include <ion/ion.hpp>
#include <entt/entt.hpp>

#include <SDL2/SDL_pixels.h>
#include <filesystem>

#include "Pipes/Tile/Tile.hpp"

namespace Pipes
{
struct TileSettings
{
    SDL_Color static_color{ 0x42, 0x87, 0xf5, 0xff };
    SDL_Color placeable_color{ 0x9d, 0xbe, 0xf5, 0xff };
    SDL_Color distant_color{ 0xd3, 0xd3, 0xd3, 0xff };
};
}

namespace ion
{
template<>
inline auto reflect<Pipes::TileSettings>()
{
    using namespace entt::literals;
    return entt::meta_factory<Pipes::TileSettings>{}
        .type("Pipes::TileSettings"_hs)
        .data<&Pipes::TileSettings::static_color>("static-color"_hs)
        .data<&Pipes::TileSettings::placeable_color>("placeable-color"_hs)
        .data<&Pipes::TileSettings::distant_color>("distant-color"_hs);
}
}

namespace prefab {

/** A resource manager for tile surfaces */
class tile : public ion::isotope {
public:
    /** Create a tile prefab with the specified colors */
    explicit tile(std::string_view images_path);

    /** The background color for static tiles */
    inline SDL_Color const & static_color() const { return settings.static_color; }
    /** The background color for placeable tiles */
    inline SDL_Color const & placeable_color() const { return settings.placeable_color; }
    /** The background color for distant tiles */
    inline SDL_Color const & distant_color() const { return settings.distant_color; }

    Pipes::TileMap loaded_tiles;
private:
    Pipes::TileSettings settings;
};
}
