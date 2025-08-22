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

    // initialize the random engine with a random seed
      _rng{std::random_device{}()},

    // interface between grid-space and pixel-space
    //   100 seems like a decent unit-size for now
      board(systems::grid{width, height, 100},
            Pipes::TileMap(ion::asset_loader{}, "tiles")),
      deck(_rng, 11u),
      hand(board)
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

    // create a random initial tile in the middle of the screen
    board.place_tile(board.draw_from_deck(deck, board.world_space.center()));
}

void pipes::run()
{
    const SDL_Point mouse = board.world_space.nearest_point(ion::input::mouse_position());
    hand.current_tile = board.draw_from_deck(deck, mouse);

    // bind the mouse to the tile hand
    _events.on_mouse_scroll().connect<&Pipes::Hand::on_cursor_scrolled>(hand);
    _events.on_mouse_moved().connect<&Pipes::Hand::on_cursor_moved>(hand);

    // place the tile associated with the mouse when clicked
    _events.on_mouse_up().connect<&pipes::on_mouse_clicked>(this);

    // create the window and run the game
    while (not ion::input::has_quit()) {

        // process any events then render the window
        _events.poll();
        board.render(_window);
    }
}

void pipes::on_mouse_clicked()
{
    if (not hand.current_tile)
    {
        return;
    }
    const SDL_Point position = board.entities.get<component::position>(*hand.current_tile);
    if (board.has_tile(position.x, position.y) or not board.has_adjacent_tile(position.x, position.y))
    {
        return;
    }
    board.place_tile(*hand.current_tile);
    if (deck.is_empty())
    {
        hand.current_tile = std::nullopt;
        return;
    }
    hand.current_tile = board.draw_from_deck(deck, position);
}