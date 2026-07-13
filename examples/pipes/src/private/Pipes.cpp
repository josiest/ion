#include "Pipes/Pipes.hpp"

#include <filesystem>
#include <ion/engine.hpp>
#include <ion/editor.hpp>
#include <ion/input/axis.hpp>
#include <iostream>

#include <ion/serialization/paths.hpp>
#include <SDL3/SDL_log.h>

namespace fs = std::filesystem;
namespace ranges = std::ranges;
ion::editor * GEditor = nullptr;

int main(int argc, char * argv[])
{
    const fs::path root_dir = ion::paths::root_dir();
    ion::editor_settings::config_path((root_dir/"resources/settings.yaml").string());
    SDL_Log("Config Dir: %s\n", ion::editor_settings::config_path().data());

    auto editor = ion::editor::initialize();
    if (not editor) { return EXIT_FAILURE; }
    GEditor = editor.get();

    const auto editor_settings = ion::editor_settings::load();
    SDL_Log("Editor name: %s\n", editor_settings.window_name.c_str());

    const Pipes::GameSettings game_settings;
    constexpr Pipes::TileSettings tile_settings;
    Pipes::App game{ game_settings, tile_settings };

    // otherwise run the game
    game.start();
    while (not editor->has_quit())
    {
        ion::sdl_events::poll();
        game.update();
    }
    return EXIT_SUCCESS;
}

Pipes::App::App(const GameSettings & game_settings,
                const TileSettings & tile_settings)

    : _rng(std::random_device{}()),

      board(TileMap(game_settings.tiles_directory), tile_settings),

      deck(_rng, game_settings.deck_size),
      hand(board)
{

    board.background_color = game_settings.background_color;
    SDL_Point window_size;
    SDL_GetWindowSize(GEditor->window.get(), &window_size.x, &window_size.y);

    board.transform.translate(static_cast<float>(window_size.x)/2.f,
                              static_cast<float>(window_size.y)/2.f);

    board.transform.scale_by(static_cast<float>(game_settings.unit_size),
                            -static_cast<float>(game_settings.unit_size));
    board.pixel_to_board = glm::inverse(board.transform.global_basis());

    // create a random initial tile in the middle of the screen
    board.place_tile(board.draw_from(deck, SDL_Point{ 0, 0 }));
}

void Pipes::App::start()
{
    const SDL_Point mouse = board.nearest_point(ion::input::mouse_position());
    hand.current_tile = board.draw_from(deck, mouse);

    // bind the mouse to the tile hand
    ion::sdl_events::on_mouse_scroll().connect<&Hand::on_cursor_scrolled>(hand);
    ion::sdl_events::on_mouse_moved().connect<&Hand::on_cursor_moved>(hand);

    // place the tile associated with the mouse when clicked
    ion::sdl_events::on_mouse_up().connect<&App::on_mouse_clicked>(this);
}

void Pipes::App::update()
{
    board.render(GEditor->window.get());
}

void Pipes::App::on_mouse_clicked()
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