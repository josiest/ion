#pragma once
#include "grid.hpp"
#include <entt.hpp>
#include <SDL2/SDL.h>

void bind_to_mouse(grid const & world_space, entt::registry & entities, 
                   entt::entity tile);
