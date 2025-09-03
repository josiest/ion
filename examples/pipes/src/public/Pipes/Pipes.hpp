#pragma once

#include "Pipes/Board.hpp"
#include "Pipes/Deck.hpp"
#include "Pipes/Hand.hpp"
#include "Pipes/Tile/TileSettings.hpp"

#include <ion/ion.hpp>
#include <entt/entt.hpp>

#include <cstdint>
#include <random>

namespace Pipes
{
struct WindowSettings
{
    std::string name = "Pipes";
    std::uint32_t width = 800;
    std::uint32_t height = 600;
};

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
auto reflect<Pipes::WindowSettings>()
{
    using namespace entt::literals;
    return entt::meta_factory<Pipes::WindowSettings>{}
        .type("Pipes::WindowSettings"_hs)
        .data<&Pipes::WindowSettings::name>("name"_hs)
        .data<&Pipes::WindowSettings::width>("width"_hs)
        .data<&Pipes::WindowSettings::height>("height"_hs);
}

template<>
auto reflect<Pipes::GameSettings>()
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

class App : public ion::isotope {
public:
    using engine_t = std::mt19937;

    App(std::uint32_t width, std::uint32_t height);
    App(const ion::asset_loader & asset_loader,
        const Pipes::WindowSettings & window_settings,
        const Pipes::GameSettings & game_settings,
        const Pipes::TileSettings & tile_settings);
    void run();
private:
    void on_mouse_clicked();

    // events
    ion::event_sink _events;

    // initialize sdl before other sdl resources
    ion::sdl_context _sdl;
    ion::software_renderer _window;

    // ecs
    engine_t _rng;

    // tile
    Pipes::Board board;
    Pipes::Deck deck;
    Pipes::Hand hand;
};
}
