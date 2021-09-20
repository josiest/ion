#pragma once
#include "entities/player.hpp"
#include "components.hpp"

#include <entt/entity/registry.hpp>
#include <SDL2/SDL.h>

#include <random>
#include <vector>

class munchable_factory {
public:
    using engine_t = std::mt19937;
    munchable_factory(size_t w, size_t h, engine_t rng);
    entt::entity make_munchable(entt::registry & entities, entt::entity player,
                                prefab::player const & player_settings);
private:
    SDL_Rect _bounds;
    engine_t _rng;
    std::vector<component::color> _colors;
};
