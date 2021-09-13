#pragma once
#include "entities/tile.hpp"
#include "systems/grid.hpp"

#include <SDL2/SDL_surface.h>
#include <entt.hpp>

void render_grid_tiles(SDL_Surface * screen, grid const & world_space,
                       entt::registry & registry, tiles::map & tilemap);
