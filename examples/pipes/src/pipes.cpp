#include "pipes.hpp"

#include "components.hpp"
#include "entities/tile.hpp"

#include "systems/grid.hpp"
#include "systems/tile.hpp"

#include <ion/ion.hpp>
#include <SDL2/SDL.h>
#include <entt.hpp>

#include <memory>
#include <random>

pipes::pipes(size_t width, size_t height)

    : _width{width}, _height{height},

      // interface between grid-space and pixel-space
      //   origin is at (0, height) because pixel-space vertical axis is weird
      //   50 seems like a decent unit-size for now
      _world_space{0, static_cast<int>(height), width, height, 50},

      // intialize the random engine with a random seed
      _rng{std::random_device{}()}
{}

void pipes::run()
{
    entt::registry entities;

    // create a random tile associated with the mouse and save a reference
    auto mouse_tile = tiles::make(entities, tiles::random_name(_rng),
                                            tiles::random_rotation(_rng),
                                            ion::input::mouse_position());

    // quit when the window is quit
    _events.subscribe(SDL_QUIT, &ion::input::quit_on_event);

    // rotate the tile associated with the mouse when scrolled
    _events.subscribe_functor(SDL_MOUSEWHEEL,
            [this, &entities, mouse_tile](SDL_Event const & event) {
            rotate_tile(entities, mouse_tile, event);
            });

    // place the tile associated with the mouse when clicked
    _events.subscribe_functor(SDL_MOUSEBUTTONUP,
            [this, &entities, mouse_tile](SDL_Event const & event) {
            place_tile(entities, mouse_tile, event);
            });

    // create the window and run the game
    auto window = ion::basic_blit_window("Pipes", _width, _height);
    while (!ion::input::has_quit()) {

        // bind the mouse-tile entity to the mouse position
        bind_to_mouse(entities, mouse_tile);

        // process any events then render the window
        _events.process_queue();
        render(window, entities);
    }
}

int main()
{
    pipes game(800, 600);
    game.run();
}
