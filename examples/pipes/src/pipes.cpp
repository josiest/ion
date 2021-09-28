#include "pipes.hpp"

#include "systems.hpp"
#include "entities.hpp"

#include <ion/ion.hpp>
#include <entt/entt.hpp>
#include <SDL.h>

#include <memory>
#include <random>
#include <iostream>

#include <algorithm>

namespace ranges = std::ranges;

int main()
{
    pipes game{800, 600};

    // crash if the game failed to initialize
    if (not game) {
        std::cout << game.error() << std::endl;
        return 1;
    }
    // otherwise run the game
    game.run();
}

void pipes::run()
{
    // create a random tile associated with the mouse
    auto const mouse_tile = _tile_prefab.random_dynamic(
        _entities, _rng, ion::input::mouse_position()
    );

    // rotate the tile associated with the mouse when scrolled
    _events.subscribe_functor(SDL_MOUSEWHEEL,
        [this, mouse_tile](SDL_Event const & event) {
            systems::rotate_tile(_entities, mouse_tile, event.wheel.y);
        });

    // place the tile associated with the mouse when clicked
    _events.subscribe_functor(SDL_MOUSEBUTTONUP,
        [this, mouse_tile](SDL_Event const & event) {
            _tile_prefab.static_copy(_entities, _placed_tiles, mouse_tile, _rng);
        });

    // create the window and run the game
    while (not ion::input::has_quit()) {

        // bind the mouse-tile entity to the mouse position
        systems::bind_to_mouse(_entities, mouse_tile, _world_space);

        // process any events then render the window
        _events.process_queue();
        systems::render(_window, _world_space, _entities,
                        _tile_prefab, _placed_tiles, mouse_tile);
    }
}

pipes::pipes(std::uint32_t width, std::uint32_t height)
    : _window{"Pipes", width, height},

      // interface between grid-space and pixel-space
      //   100 seems like a decent unit-size for now
      _world_space{width, height, 100},

      // intialize the random engine with a random seed
      _rng{std::random_device{}()},

      // tell the tile prefab where to find the tile images
      // and what colors should be used
      _tile_prefab{"../resources", {0x42, 0x87, 0xf5, 0xff},    // static color
                                   {0x9d, 0xbe, 0xf5, 0xff},    // placeable
                                   {0xd3, 0xd3, 0xd3, 0xff}}    // distant
{
    // quit when SDL quit event is triggered
    _events.subscribe(SDL_QUIT, &ion::input::quit_on_event);

    // make sure SDL resources initialized properly
    if (not _sdl) {
        _error = "Couldn't initialize SDL: " + _sdl.error();
        return;
    }
    if (not _window) {
        _error = "Couldn't create a window: " + _window.error();
        return;
    }

    // make sure the tile images were loaded properly
    if (not _tile_prefab) {
        _error = "Couldn't load tiles:\n" + _tile_prefab.error();
    }

    // create a random initial tile in the middle of the screen
    auto init_point = _world_space.nearest_point(width/2, height/2);
    _tile_prefab.random_static(_entities, _placed_tiles, init_point, _rng);
}
