#pragma once
#include "tiles.hpp"
#include "grid.hpp"

#include <ion/render/window.hpp>
#include <entt.hpp>

void render_grid_tiles(ion::Window & window, entt::registry & registry,
                       Grid const & grid, tiles::TileMap & tilemap);
