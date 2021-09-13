#include "entities/tile.hpp"

#include "systems/render.hpp"
#include "systems/input.hpp"
#include "systems/grid.hpp"

#include <ion/ion.hpp>
#include <SDL2/SDL.h>
#include <entt.hpp>

#include <memory>

class pipes {
public:
    pipes(int width, int height)

          // create a basic window called "Pipes" with the specified dimensions
        : _window{std::make_unique<ion::blit_window>(
                ion::load_window("Pipes", width, height)
          )},

          // interface between grid-space and pixel-space
          //   origin is at (0, height) because pixel-space vertical axis is weird
          //   50 seems like a decent unit-size for now
          _world_space{0, height, width, height, 50},

          // initialize ECS registry and create tile that's bound to mouse
          //   for now use Bend tile as initial value,
          //   but in the future this should be randomized
          //
          //   "right" is good for a default rotation
          //
          //   (-1, -1) is not visible on the grid:
          //   good intial point before binding it to the mouse
          //   in the future it may be a good idea to intialize it to mouse position
          //   this would require making a function to do this
          //   it may be a good idea to make such a function part of the ion framework
          _mouse_tile{tiles::make(
              _entities, tiles::Name::Bend, tiles::Rotation::Right, -1, -1
          )}
    {
        _events.subscribe(SDL_QUIT, &ion::input::quit_on_event);
    }
    ~pipes() { /*_tiles.clear();*/ _window.reset(); SDL_Quit(); }

    void render()
    {
        // clear the screen
        auto screen = _window->sdl_surface();
        SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0, 0, 0));

        // draw the tiles and update the window
        render_grid_tiles(screen, _world_space, _entities, _tiles);
        SDL_UpdateWindowSurface(_window->sdl_window());
    }

    void run()
    {
        while (!ion::input::has_quit()) {
            // bind the mouse-tile entity to the mouse position
            bind_to_mouse(_world_space, _entities, _mouse_tile);

            _events.process_queue();
            render();
        }
    }
private:
    std::unique_ptr<ion::blit_window> _window;
    ion::event_system _events;

    tiles::map _tiles;
    grid _world_space;

    entt::registry _entities;
    entt::entity _mouse_tile;
};

int main()
{
    pipes game(800, 600);
    game.run();
}
