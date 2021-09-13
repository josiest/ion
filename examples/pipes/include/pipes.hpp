#pragma once

#include "entities/tile.hpp"
#include "systems/grid.hpp"

#include <ion/ion.hpp>
#include <SDL2/SDL.h>
#include <entt.hpp>

#include <unordered_set>
#include <random>

namespace std {
template<> struct hash<SDL_Point> {
    size_t operator()(SDL_Point const & p) const
    {
        return hash<int>{}(p.x) ^ hash<int>{}(p.y);
    }
};
}

inline bool operator==(SDL_Point const & p, SDL_Point const & q)
{
    return p.x == q.x && p.y == q.y;
}

class pipes {
public:
    using engine_t = std::mt19937;
    pipes(size_t width, size_t height);
    inline ~pipes() { _tiles.clear(); SDL_Quit(); }
    void run();
private:
    size_t _width, _height;
    ion::event_system _events;

    tiles::map _tiles;
    grid _world_space;

    engine_t _rng;

    std::unordered_set<SDL_Point> _placed_tiles;

    // render systems
    void render(ion::window & window, entt::registry & entities);
    void render_grid_tiles(ion::window & window, entt::registry & entities);

    // input systems
    void bind_to_mouse(entt::registry & entities, entt::entity tile);

    void rotate_tile(entt::registry & entities, entt::entity tile,
                     SDL_Event const & event);

    void place_tile(entt::registry & entities, entt::entity tile,
                    SDL_Event const & event);
};
