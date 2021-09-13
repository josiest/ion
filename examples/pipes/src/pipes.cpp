#include "components.hpp"
#include "entities/tile.hpp"

#include "systems/render.hpp"
#include "systems/input.hpp"
#include "systems/grid.hpp"

#include <ion/ion.hpp>
#include <SDL2/SDL.h>
#include <entt.hpp>

#include <memory>
#include <random>

class pipes {
public:
    using engine_t = std::mt19937;
    pipes(int width, int height)

          // create a basic window called "Pipes" with the specified dimensions
        : _window{std::make_unique<ion::blit_window>(
                ion::load_window("Pipes", width, height)
          )},

          // interface between grid-space and pixel-space
          //   origin is at (0, height) because pixel-space vertical axis is weird
          //   50 seems like a decent unit-size for now
          _world_space{0, height, width, height, 50},

          // intialize the random engine with a random seed
          _rng{std::random_device{}()},

          // initialize ECS registry and create tile that's bound to mouse
          _mouse_tile{tiles::make(_entities,
                  tiles::random_name(_rng),
                  tiles::random_rotation(_rng),
                  ion::input::mouse_position()
          )}
    {
        _events.subscribe(SDL_QUIT, &ion::input::quit_on_event);
        _events.subscribe_functor(SDL_MOUSEWHEEL,
                [this](SDL_Event const & event) { rotate_tile(event); });
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

    engine_t _rng;

    entt::registry _entities;
    entt::entity _mouse_tile;

    void rotate_tile(SDL_Event const & event);
};

pipes GAME(800, 600);
void pipes::rotate_tile(SDL_Event const & event)
{
    auto & tile = _entities.get<Tile>(_mouse_tile);
    if (event.wheel.y > 0) { increment_rotation(tile.rotation); }
    else if (event.wheel.y < 0) { decrement_rotation(tile.rotation); }
}

int main()
{
    GAME.run();
}
