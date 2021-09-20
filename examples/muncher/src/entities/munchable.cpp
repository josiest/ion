#include "entities/munchable.hpp"
#include "entities/player.hpp"
#include "components.hpp"

#include <entt/entity/registry.hpp>

#include <random>
#include <cmath>
#include <cstdint>

munchable_factory::munchable_factory(size_t w, size_t h, engine_t rng)

    : _bounds{0, 0, static_cast<int>(w), static_cast<int>(h)}, _rng{rng}
{
    _colors = std::vector<component::color> {
        {0xf3, 0x91, 0x89}, {0xbb, 0x80, 0x82},
        {0x6e, 0x75, 0x82}, {0x04, 0x65, 0x82}
    };
}

entt::entity munchable_factory::make_munchable(
        entt::registry & entities, entt::entity player,
        prefab::player const & player_settings)
{
    // create the munchable entity
    auto munchable = entities.create();

    // the distribution for the x and y position
    std::uniform_real_distribution<float> xdist(_bounds.x, _bounds.x+_bounds.w);
    std::uniform_real_distribution<float> ydist(_bounds.y, _bounds.y+_bounds.h);

    // the choices for the x and y positions (must be on a boundary)
    std::vector<float> xvals{
        static_cast<float>(_bounds.x), static_cast<float>(_bounds.x+_bounds.w),
        xdist(_rng), xdist(_rng)
    };
    std::vector<float> yvals{
        ydist(_rng), ydist(_rng),
        static_cast<float>(_bounds.y), static_cast<float>(_bounds.y+_bounds.h)
    };

    // choose a random pair from xvals and yvals
    std::uniform_int_distribution<size_t> pos_dist(0, xvals.size()-1);
    size_t const i = pos_dist(_rng);
    float const x = xvals[i];
    float const y = yvals[i];

    // get the size of the player as a mean for the size of the munchable
    auto const player_box = player_settings.try_get_bbox(entities, player);

    // generate a random size
    std::normal_distribution<float> size_dist(player_box.size, 5.f);
    float const size = std::abs(static_cast<int>(std::round(size_dist(_rng))));

    // add the new bbox component to the munchable entitiy
    entities.emplace<component::bbox>(munchable, x, y, size);

    // the distributions for velocity speed and angle
    std::uniform_real_distribution<float> speed_dist(100.f, 200.f);
    float const speed = speed_dist(_rng);

    // calculate the angle to the player
    float const dx = player_box.x - x;
    float const dy = player_box.y - y;
    float const dist = std::sqrt(dx*dx + dy*dy);
    float const angle_to_player = std::atan2(dy/dist, dx/dist);

    // generate a random angle
    float const pi = std::atan(1.f)*4.f;
    std::normal_distribution<float> angle_dist(angle_to_player, pi/6.f);
    float const phi = angle_dist(_rng);

    // calculate the velocity from the random speed and angle
    entities.emplace<component::velocity>(munchable, speed*std::cos(phi),
                                                     speed*std::sin(phi));

    // choose a color
    std::uniform_int_distribution<size_t> color_dist(0, _colors.size()-1);
    auto color = _colors[color_dist(_rng)];

    entities.emplace<component::color>(munchable,
                                       color.r, color.g, color.b);

    entities.emplace<component::munchable>(munchable);

    return munchable;
}
