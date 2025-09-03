#pragma once
#include <ion/mylar.hpp>
#include <SDL2/SDL_pixels.h>

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
    reflect<SDL_Color>();
    return entt::meta_factory<Pipes::TileSettings>{}
        .type("Pipes::TileSettings"_hs)
        .data<&Pipes::TileSettings::static_color>("static-color"_hs)
        .data<&Pipes::TileSettings::placeable_color>("placeable-color"_hs)
        .data<&Pipes::TileSettings::distant_color>("distant-color"_hs);
}
}

