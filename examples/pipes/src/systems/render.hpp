#pragma once
#include "components.hpp"

#include "systems/point.hpp"
#include "systems/grid.hpp"

#include "Pipes/Tile/TileMap.hpp"
#include "Pipes/Tile/Tile.hpp"
#include "Pipes/TileSettings.hpp"

#include <ion/ion.hpp>
#include <entt/entt.hpp>
#include <SDL2/SDL.h>

namespace systems {

void render(ion::window_resource auto & window,
            systems::grid const & world_space,
            entt::registry & entities,
            const Pipes::TileMap & loaded_tiles)
{
    // clear the screen
    auto screen = SDL_GetWindowSurface(window);
    SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0, 0, 0));

    // draw the tiles and update the window
    entities.view<Pipes::Component::Tile, component::position>()
            .each([&](const auto & tile, const auto & position)
    {
        // get the sdl surface to render from and the grid square to render to
        SDL_Surface * tile_surface = loaded_tiles.image_for(tile.name, tile.rotation);
        SDL_Rect grid_square = world_space.unit_square(position);

        // color the background and draw the tile
        SDL_FillRect(screen, &grid_square, SDL_MapRGB(screen->format, tile.color.r, tile.color.g, tile.color.b));
        SDL_BlitScaled(tile_surface, nullptr, screen, &grid_square);
    });
    SDL_UpdateWindowSurface(window);
}
}
