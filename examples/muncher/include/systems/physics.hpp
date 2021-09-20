#pragma once
#include <entt/entity/registry.hpp>
#include <ion/input.hpp>
#include <SDL2/SDL.h>

namespace systems {

void accelerate_player(entt::registry & registry, entt::entity player,
                       ion::input::axis2d const & input, float dt);

void move_munchies(entt::registry & registry, float dt);

bool collides_with(SDL_Rect const & a, SDL_Rect const & b);
void devour(entt::registry & registry, entt::entity player);

}
