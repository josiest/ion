#include "pipes.hpp"
#include "error.hpp"

#include "systems/tile.hpp"
#include "systems/input.hpp"
#include "systems/render.hpp"
#include "systems/resource.hpp"

#include "entities/tile.hpp"

#include <ion/ion.hpp>
#include <SDL2/SDL.h>
#include <entt.hpp>

#include <memory>
#include <random>
#include <iostream>

#include <algorithm>

namespace ranges = std::ranges;

int main()
{
    pipes game{800, 600};

    // crash if the game failed to initialize
    if (game.bad()) {
        std::cout << get_error() << std::endl;
        return 1;
    }
    // otherwise run the game
    game.run();
}

pipes::pipes(std::uint32_t width, std::uint32_t height)

    : _window{"Pipes", width, height},
      _width{width}, _height{height},

      _tiles{tiles::load_map()},

      // interface between grid-space and pixel-space
      //   origin is at (0, height) because pixel-space vertical axis is weird
      //   50 seems like a decent unit-size for now
      _world_space{0, static_cast<int>(height), width, height, 100},

      // intialize the random engine with a random seed
      _rng{std::random_device{}()}
{
    if (not _sdl) {
        set_error(_sdl.error());
        return;
    }
    if (not _window) {
        set_error(_window.error());
        return;
    }
    auto tile_is_bad = [](auto const & pair) { return not *pair.second; };
    if (_tiles.empty() or ranges::any_of(_tiles, tile_is_bad)) {
        return;
    }
}

void pipes::run()
{
    entt::registry entities;

    // create a random initial tile in the middle of the screen
    auto init_point = _world_space.nearest_point(
            SDL_Point{static_cast<int>(_width)/2, static_cast<int>(_height)/2});

    auto init_tile = tiles::make_static(entities,
            tiles::random_name(_rng), tiles::random_rotation(_rng),
            init_point.x, init_point.y);

    _placed_tiles.insert(init_point);

    // create a random tile associated with the mouse and save a reference
    auto mouse_tile = tiles::make(entities, tiles::random_name(_rng),
                                            tiles::random_rotation(_rng),
                                            ion::input::mouse_position());

    // quit when the window is quit
    _events.subscribe(SDL_QUIT, &ion::input::quit_on_event);

    // rotate the tile associated with the mouse when scrolled
    _events.subscribe_functor(SDL_MOUSEWHEEL,
            [&entities, mouse_tile](SDL_Event const & event) {
            rotate_tile(entities, mouse_tile, event);
            });

    // place the tile associated with the mouse when clicked
    _events.subscribe_functor(SDL_MOUSEBUTTONUP,
            [this, &entities, mouse_tile](SDL_Event const & event) {
            place_tile(entities, mouse_tile, *this, event);
            });

    // create the window and run the game
    while (!ion::input::has_quit()) {

        // bind the mouse-tile entity to the mouse position
        bind_to_mouse(entities, mouse_tile, _world_space);

        // process any events then render the window
        _events.process_queue();
        render(_window, entities, mouse_tile, *this);
    }
}
