#include "pipes.hpp"

#include "systems/tile.hpp"
#include "systems/input.hpp"
#include "systems/render.hpp"

#include "entities/tile.hpp"

#include <ion/ion.hpp>
#include <SDL2/SDL.h>
#include <entt/entity/registry.hpp>

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

pipes::pipes(std::uint32_t width, std::uint32_t height)
    : _window{"Pipes", width, height},

      // tile creation infromation
      _tile_prefab{"../resources", {0x42, 0x87, 0xf5, 0xff},    // static
                                   {0x9d, 0xbe, 0xf5, 0xff},    // placeable
                                   {0xd3, 0xd3, 0xd3, 0xff}},   // distant

      // interface between grid-space and pixel-space
      //   origin is at (0, height) because pixel-space vertical axis is weird
      //   50 seems like a decent unit-size for now
      _world_space{0, static_cast<int>(height), width, height, 100},

      // intialize the random engine with a random seed
      _rng{std::random_device{}()}
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
    if (not _tile_prefab) {
        _error = "Couldn't load tiles:\n" + _tile_prefab.error();
    }
}

void pipes::run()
{
    // create a random initial tile in the middle of the screen
    int width, height;
    SDL_GetWindowSize(_window, &width, &height);
    auto init_point = _world_space.nearest_point(SDL_Point{width/2, height/2});

    auto init_tile = _tile_prefab.create_static(_entities,
            tiles::random_name(_rng), tiles::random_rotation(_rng),
            init_point.x, init_point.y);

    _placed_tiles.insert(init_point);

    // create a random tile associated with the mouse and save a reference
    auto mouse_tile = _tile_prefab.create(
        _entities, tiles::random_name(_rng),
                   tiles::random_rotation(_rng), ion::input::mouse_position()
    );

    // rotate the tile associated with the mouse when scrolled
    _events.subscribe_functor(SDL_MOUSEWHEEL,
            [this, mouse_tile](SDL_Event const & event) {
            rotate_tile(_entities, mouse_tile, event);
            });

    // place the tile associated with the mouse when clicked
    _events.subscribe_functor(SDL_MOUSEBUTTONUP,
            [this, mouse_tile](SDL_Event const & event) {
            place_tile(_entities, _tile_prefab, mouse_tile, *this, event);
            });

    // create the window and run the game
    while (!ion::input::has_quit()) {

        // bind the mouse-tile entity to the mouse position
        bind_to_mouse(_entities, mouse_tile, _world_space);

        // process any events then render the window
        _events.process_queue();
        render(_window, _entities, _tile_prefab, mouse_tile, *this);
    }
}
