#pragma once

#include "pipes.hpp"
#include <entt/entity/registry.hpp>
#include <SDL2/SDL_events.h>

void bind_to_mouse(entt::registry & entities, entt::entity tile,
                   grid const & world_space);

void rotate_tile(entt::registry & entities, entt::entity tile,
                 SDL_Event const & event);

void place_tile(entt::registry & entities, entt::entity tile,
                pipes & game, SDL_Event const & event);
