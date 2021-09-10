#pragma once
#include "components.hpp"
#include <entt/entity/registry.hpp>
#include <SDL2/SDL.h>
#include <random>
#include <vector>

class MunchableFactory {
public:
    using engine_t = std::mt19937;
    MunchableFactory(size_t w, size_t h, engine_t rng);
    entt::entity make_munchable(entt::registry & registry, entt::entity player);
    void filter(entt::registry & registry);
private:
    SDL_Rect _bounds;
    engine_t _rng;
    std::vector<Color> _colors;
};
