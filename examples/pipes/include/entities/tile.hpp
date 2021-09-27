#pragma once

#include "types/tile.hpp"
#include <SDL2/SDL_rect.h>
#include <entt/entity/registry.hpp>

namespace tiles {

constexpr SDL_Color static_bg_color = {0x42, 0x87, 0xf5, 0xff};
constexpr SDL_Color placeable_bg_color = {0x9d, 0xbe, 0xf5, 0xff};
constexpr SDL_Color distant_bg_color {0xd3, 0xd3, 0xd3, 0xff};

entt::entity make(entt::registry & entities,
                  Name name, Rotation rotation, int x, int y);

entt::entity make(entt::registry & entities,
                  Name name, Rotation rotation, SDL_Point const & p);

entt::entity make_static(entt::registry & entities,
                         Name tilename, Rotation rotation, int x, int y);

}
