#pragma once
#include "grid.hpp"
#include <entt.hpp>

void bind_to_mouse(entt::registry & registry, Grid const & grid,
                   entt::entity entity);
