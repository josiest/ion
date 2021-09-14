#pragma once

#include "pipes.hpp"
#include <entt.hpp>

void render(ion::window & window, entt::registry & entities, pipes & game);
void render_grid_tiles(ion::window & window, entt::registry & entities,
                       pipes & game);
