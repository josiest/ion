#include "pipes.hpp"

#include "systems.hpp"
#include "systems/point.hpp"

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
      _tile_prefab{"tiles"},

      deck(_rng, 11u),
      hand(_entities, _world_space)
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
    place_tile(next_tile_from_deck(_world_space.center()));
}

void pipes::run()
{
    hand.current_tile = next_tile_from_deck(ion::input::mouse_position());

    // bind the mouse to the tile hand
    _events.on_mouse_scroll().connect<&Pipes::TileHand::on_cursor_scrolled>(hand);
    _events.on_mouse_moved().connect<&Pipes::TileHand::on_cursor_moved>(hand);

    // place the tile associated with the mouse when clicked
    _events.on_mouse_up().connect<&pipes::on_mouse_clicked>(this);

    // create the window and run the game
    while (not ion::input::has_quit()) {

        // process any events then render the window
        _events.poll();
        systems::render(_window, _world_space, _entities, _tile_prefab, _placed_tiles,
                        hand.current_tile? *hand.current_tile : entt::null);
    }
}

void pipes::on_mouse_clicked()
{
    if (not hand.current_tile)
    {
        return;
    }
    const SDL_Point position = _entities.get<component::position>(*hand.current_tile);
    if (_placed_tiles.contains(position) or not systems::is_adjacent(_placed_tiles, position))
    {
        return;
    }
    place_tile(*hand.current_tile);
    if (deck.is_empty())
    {
        hand.current_tile = std::nullopt;
        return;
    }
    hand.current_tile = next_tile_from_deck(position);
}

entt::entity pipes::next_tile_from_deck(const SDL_Point & position)
{
    const entt::entity next_tile = _entities.create();
    _entities.emplace<component::position>(next_tile, position.x, position.y);

    const auto [name, rotation] = deck.next_tile();
    const SDL_Color color = systems::is_adjacent(_placed_tiles, position)
                          ? _tile_prefab.placeable_color()
                          : _tile_prefab.distant_color();

    _entities.emplace<Pipes::Component::Tile>(next_tile, name, rotation, color);

    return next_tile;
}

void pipes::place_tile(const entt::entity tile_id)
{
    const SDL_Point position = _entities.get<component::position>(tile_id);
    _entities.get<Pipes::Component::Tile>(tile_id).color = _tile_prefab.static_color();
    _entities.emplace<component::static_tile>(tile_id);
    _placed_tiles.try_emplace(position, tile_id);
}