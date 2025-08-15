#pragma once

#include "components.hpp"
#include "systems.hpp"
#include "entities.hpp"

#include <ion/ion.hpp>
#include <entt/entt.hpp>
#include <entt/signal/sigh.hpp>
#include <SDL.h>

#include <string>
#include <cstdint>
#include <random>

class event_sink
{
public:
    void poll()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit.publish();
                break;

            case SDL_MOUSEWHEEL:
                mouse_scroll.publish(event.wheel.y);
                break;

            case SDL_MOUSEBUTTONUP:
                mouse_up.publish();
            }
        }
    }
    inline auto on_quit() { return entt::sink{ quit }; }
    inline auto on_mouse_scroll() { return entt::sink{ mouse_scroll }; }
    inline auto on_mouse_up() { return entt::sink{ mouse_up }; }

protected:
    entt::sigh<void()> quit;
    entt::sigh<void(int)> mouse_scroll;
    entt::sigh<void()> mouse_up;
};

/** A resource manager for all game resources */
class pipes : public ion::isotope {
public:
    using engine_t = std::mt19937;

    pipes(std::uint32_t width, std::uint32_t height);
    void run();
private:
    void rotate_tile(int dy);
    void place_tile();

    // events
    event_sink _events;

    // initialize sdl before other sdl resources
    ion::sdl_context _sdl;
    ion::software_renderer _window;
    systems::grid _world_space;

    // ecs
    engine_t _rng;
    entt::registry _entities;
    prefab::tile _tile_prefab;
    pointset _placed_tiles;

    // tile
    std::optional<entt::entity> _current_tile;
};
