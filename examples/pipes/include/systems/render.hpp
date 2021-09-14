#pragma once

#include "pipes.hpp"
#include <entt.hpp>

void render(ion::window & window, entt::registry & entities,
            entt::entity mouse_tile, pipes & game);

void render_mouse_tile(ion::window & window, entt::registry & entities,
                       entt::entity mouse_tile, pipes & game);

void render_static_tiles(ion::window & window, entt::registry & entities,
                         pipes & game);
