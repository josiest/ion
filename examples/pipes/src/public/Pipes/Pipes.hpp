#pragma once

#include "Pipes/Board.hpp"
#include "Pipes/Deck.hpp"
#include "Pipes/Hand.hpp"
#include "Pipes/Tile/TileSettings.hpp"

#include <entt/entt.hpp>

#include <random>

namespace Pipes
{
struct GameSettings
{
    std::uint32_t deck_size = 20;
    std::uint32_t unit_size = 100;
    SDL_Color background_color{ 0x0, 0x0, 0x0, 0xff };
    std::string tiles_directory = "tiles";
};
}

namespace ion
{
template<>
inline auto reflect<Pipes::GameSettings>()
{
    using namespace entt::literals;
    ion::reflect<SDL_Color>();
    return entt::meta_factory<Pipes::GameSettings>{}
        .type("Pipes::GameSettings"_hs)
        .data<&Pipes::GameSettings::deck_size>("deck-size"_hs)
        .data<&Pipes::GameSettings::unit_size>("unit-size"_hs)
        .data<&Pipes::GameSettings::background_color>("background-color"_hs)
        .data<&Pipes::GameSettings::tiles_directory>("tiles-directory"_hs);
}
}

namespace Pipes
{

class App {
public:
    using engine_t = std::mt19937;

    App(const GameSettings & game_settings,
        const TileSettings & tile_settings);

    void start();
    void update();
private:
    void on_mouse_clicked();

    // ecs
    engine_t _rng;

    // tile
    Board board;
    Deck deck;
    Hand hand;
};
}
