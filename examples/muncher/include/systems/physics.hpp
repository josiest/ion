#pragma once
#include <entt/entity/registry.hpp>
#include <ion/input.hpp>

void accelerate_player(entt::registry & registry,
                       ion::input::InputAxis2D const & axis, float dt);

void move_munchies(entt::registry & registry, float dt);
