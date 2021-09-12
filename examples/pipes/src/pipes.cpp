#include "grid.hpp"
#include "tiles.hpp"

#include "entities/tile.hpp"

#include "systems/render.hpp"
#include "systems/input.hpp"

#include <ion/ion.hpp>
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>
#include <iostream>

class Pipes {
public:
    Pipes(int width, int height)
        : _window{ion::basic_blit_window("Pipes", width, height)},
          _tiles{}, _grid{0, height, width, height, 50},
          _registry{},

          _mouse_tile{make_tile(
              _registry, tiles::TileName::Bend, tiles::Rotation::Right, -1, -1
          )}
    {
        _handler.subscribe(SDL_QUIT, &ion::input::quit_on_event);
    }
    ~Pipes() { SDL_Quit(); }

    void render()
    {
        // clear the screen
        auto screen = _window.sdl_surface();
        SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0, 0, 0));

        // draw the tiles and update the window
        render_grid_tiles(_window, _registry, _grid, _tiles);
        SDL_UpdateWindowSurface(_window.sdl_window());
    }

    void run()
    {
        while (!ion::input::has_quit()) {
            // bind the mouse-tile entity to the mouse position
            bind_to_mouse(_registry, _grid, _mouse_tile);

            _handler.process_queue();
            render();
        }
    }
private:
    ion::Window _window;
    ion::EventHandler _handler;

    tiles::TileMap _tiles;
    Grid _grid;

    entt::registry _registry;
    entt::entity _mouse_tile;
};

int main()
{
    Pipes game(800, 600);
    game.run();
}
