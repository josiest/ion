#include "pipes.hpp"

#include "systems.hpp"

#include <ion/ion.hpp>
#include <SDL2/SDL.h>

#include <random>
#include <iostream>

namespace ranges = std::ranges;

int main(int argc, char * argv[])
{
    pipes game{800, 600};

    // crash if the game failed to initialize
    if (not game) {
        std::cout << game.get_error() << std::endl;
        return EXIT_FAILURE;
    }
    // otherwise run the game
    game.run();
    return EXIT_SUCCESS;
}


pipes::pipes(std::uint32_t width, std::uint32_t height)
    : _window{ion::software_renderer::basic_window("Pipes", width, height)},

    // interface between grid-space and pixel-space
    //   100 seems like a decent unit-size for now
      _world_space{width, height, 100},

    // initialize the random engine with a random seed
      _rng{std::random_device{}()},

    // tell the tile prefab where to find the tile images
      _tile_prefab{"../resources"}
{
    // quit when SDL quit event is triggered
    _events.on_quit().connect<&ion::input::quit>();

    // make sure SDL resources initialized properly
    if (not _sdl) {
        set_error("Couldn't initialize SDL: " + _sdl.get_error());
        return;
    }
    if (not _window) {
        set_error("Couldn't create a window: " + _window.get_error());
        return;
    }

    // make sure the tile images were loaded properly
    if (not _tile_prefab) {
        set_error("Couldn't load tiles:\n" + _tile_prefab.get_error());
    }

    // create a random initial tile in the middle of the screen
    auto init_point = _world_space.nearest_point(width/2, height/2);
    _tile_prefab.random_static(_entities, _placed_tiles, init_point, _rng);
}

void pipes::run()
{
    // create a random tile associated with the mouse
    _current_tile = _tile_prefab.random_dynamic(
        _entities, _rng, ion::input::mouse_position()
    );

    // rotate the tile associated with the mouse when scrolled
    _events.on_mouse_scroll().connect<&pipes::rotate_tile>(this);
    // place the tile associated with the mouse when clicked
    _events.on_mouse_up().connect<&pipes::place_tile>(this);

    // create the window and run the game
    while (not ion::input::has_quit()) {

        // bind the mouse-tile entity to the mouse position
        systems::bind_to_mouse(_entities, *_current_tile, _world_space);

        // process any events then render the window
        _events.poll();
        systems::render(_window, _world_space, _entities,
                        _tile_prefab, _placed_tiles, *_current_tile);
    }
}

void pipes::rotate_tile(int dy)
{
    systems::rotate_tile(_entities, *_current_tile, dy);
}

void pipes::place_tile()
{
    _tile_prefab.static_copy(_entities, _placed_tiles, *_current_tile, _rng);
}