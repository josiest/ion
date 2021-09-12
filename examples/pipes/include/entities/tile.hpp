#pragma once
#include "tiles.hpp"
#include <entt.hpp>

entt::entity make_tile(entt::registry & registry,
                       tiles::TileName name, tiles::Rotation rot,
                       int x, int y);

void move_tile(entt::registry & registry,
               entt::entity tile, int x, int y);

void rotate_tile(entt::registry & registry,
                 entt::entity tile, tiles::Rotation rot);
