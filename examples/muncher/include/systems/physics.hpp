#pragma once

#include "components.hpp"

#include <entt/entity/registry.hpp>
#include <ion/input.hpp>

namespace systems {

void accelerate_player(entt::registry & registry, entt::entity player,
                       ion::input::axis2d const & input, float dt);

void move_munchies(entt::registry & registry, float dt);

bool collides_with(component::bbox const & a, component::bbox const & b);
}
