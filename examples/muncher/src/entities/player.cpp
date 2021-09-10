#include "entities/player.hpp"
#include "components.hpp"
#include <SDL2/SDL.h>
#include <entt/entity/registry.hpp>

entt::entity make_player(entt::registry & registry, float x, float y, int size)
{
    auto player = registry.create();
    registry.emplace<Position>(player, x, y);
    registry.emplace<Velocity>(player, 0.f, 0.f);
    registry.emplace<Size>(player, size);
    Uint8 r = 0, g = 0, b = 0, a = 0xff;
    registry.emplace<Color>(player, r, g, b, a);
    registry.emplace<DynamicMover>(player, 160.f);
    registry.emplace<Player>(player);
    return player;
}
