#include "pipes.hpp"
#include <yaml-cpp/yaml.h>
#include <iostream>

namespace ranges = std::ranges;

int main(int argc, char * argv[])
{
    ion::asset_loader asset_loader;
    Pipes::WindowSettings window_settings;
    Pipes::GameSettings game_settings;
    Pipes::TileSettings tile_settings;
    try
    {
        auto config = YAML::LoadFile("resources/settings.yaml");
        window_settings = asset_loader.from_config<Pipes::WindowSettings>(config["window"]);
        game_settings = asset_loader.from_config<Pipes::GameSettings>(config["game"]);
        tile_settings = asset_loader.from_config<Pipes::TileSettings>(config["tile"]);
    }
    catch (const YAML::Exception & error)
    {
        std::cout << error.what() << "\n";
    }

    pipes game{ asset_loader, window_settings, game_settings, tile_settings };

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
      board(Pipes::Grid{width, height, 100},
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
    board.place_tile(board.draw_from(deck, board.world_space.center()));
}

pipes::pipes(const ion::asset_loader & asset_loader,
             const Pipes::WindowSettings & window_settings,
             const Pipes::GameSettings & game_settings,
             const Pipes::TileSettings & tile_settings)

    : _window(ion::software_renderer::basic_window(window_settings.name,
                                                   window_settings.width,
                                                   window_settings.height)),

      _rng(std::random_device{}()),

      board(Pipes::Grid(window_settings.width, window_settings.height, game_settings.unit_size),
            Pipes::TileMap(asset_loader, game_settings.tiles_directory),
            tile_settings),

      deck(_rng, game_settings.deck_size),
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
    std::cout << "deck size: " << game_settings.deck_size << "\n";
    board.background_color = game_settings.background_color;

    // create a random initial tile in the middle of the screen
    board.place_tile(board.draw_from(deck, board.world_space.center()));
}

void pipes::run()
{
    const SDL_Point mouse = board.world_space.nearest_point(ion::input::mouse_position());
    hand.current_tile = board.draw_from(deck, mouse);

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
    const SDL_Point position = hand.current_tile->position();
    if (board.has_tile(position) or not board.has_adjacent_tile(position))
    {
        return;
    }
    board.place_tile(std::move(*hand.current_tile));
    if (deck.is_empty())
    {
        hand.current_tile = std::nullopt;
        return;
    }
    hand.current_tile = board.draw_from(deck, position);
}