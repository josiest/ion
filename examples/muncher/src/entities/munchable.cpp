#include "entities/munchable.hpp"
#include "entities/player.hpp"
#include "components.hpp"

#include <entt/entity/registry.hpp>

#include <random>
#include <cstdint>
#include <utility>

#include <cmath>
#include <numbers>

namespace prefab {

using namespace std::numbers;
munchable::munchable(std::uint32_t width, std::uint32_t height) noexcept

      // define the bounds where the munchable can be created
    : _width{width}, _height{height},

      // some arbitrary colors to choose from that look nice
      _colors{{0xf3, 0x91, 0x89}, {0xbb, 0x80, 0x82},
              {0x6e, 0x75, 0x82}, {0x04, 0x65, 0x82}},

      // these seem like good speeds to vary between
      _minspeed{100.f}, _maxspeed{200.f},

      // the variation of the angle in radians
      _angle_variation{pi_v<float>/6.f},

      // the variation of the munchable size
      _size_variation{.8f},

      // how likely it is for munchable to be created
      //    at any given millisecond
      _munchtime_likelihood{.01f}
{}

entt::entity
munchable::create(entt::registry & entities, entt::entity playerid,
                  player const & player_settings, engine_t rng) const noexcept
{
    // get the size of the player as a mean for the size of the munchable
    auto const player_box = player_settings.try_get_bbox(entities, playerid);

    // create the munchable entity
    auto munchableid = entities.create();

    // assign the munchable a random bbox component
    auto const & munchable_box = random_bbox(entities, munchableid,
                                             player_box, rng);

    // assign the munchable a random velocity and color component
    random_velocity(entities, munchableid, munchable_box, player_box, rng);
    random_color(entities, munchableid, rng);

    // tag the munchable and return its id
    entities.emplace<component::munchable>(munchableid);
    return munchableid;
}

bool munchable::should_munch(float dt, engine_t & rng) const noexcept
{
    // convert dt to milliseconds
    dt *= 1000.f;

    // coin flip distribution determine wether to munch
    std::binomial_distribution<bool> is_munchtime{1, _munchtime_likelihood*dt};
    return is_munchtime(rng);
}

component::bbox &
munchable::random_bbox(entt::registry & entities, entt::entity munchableid,
                       component::bbox const & player_box,
                       engine_t & rng) const noexcept
{
    // the distribution for the x and y position
    std::uniform_real_distribution<float> xdist(0.f, _width);
    float const x = xdist(rng);

    std::uniform_real_distribution<float> ydist(0.f, _height);
    float const y = ydist(rng);

    // the choices for the positions (must be on a boundary)
    using point = std::pair<float, float>;
    std::vector<point> positions{{0, y}, {_width, y}, {x, 0}, {x, _height}};

    // choose a random position
    std::uniform_int_distribution<std::size_t> idist(0, positions.size()-1);
    size_t const i = idist(rng);

    // use lognomal distribution to generate a random size to ensure positivity
    //  player_box.size is assumed to be positive
    std::lognormal_distribution size_dist{std::log(player_box.size),
                                          std::log(_size_variation)};

    // add the new bbox component to the munchable entitiy
    return entities.emplace<component::bbox>(munchableid,
            positions[i].first, positions[i].second, size_dist(rng));
}

component::velocity &
munchable::random_velocity(entt::registry & entities, entt::entity munchableid,
                           component::bbox const & munchable_box,
                           component::bbox const & player_box,
                           engine_t & rng) const noexcept
{
    // the distributions for velocity speed and angle
    std::uniform_real_distribution speed_dist(_minspeed, _maxspeed);
    float const speed = speed_dist(rng);

    // calculate the angle to the player
    float const dx = player_box.x - munchable_box.x;
    float const dy = player_box.y - munchable_box.y;
    float const dist = std::sqrt(dx*dx + dy*dy);
    float const angle_to_player = std::atan2(dy/dist, dx/dist);

    // generate a random angle
    float const pi = std::atan(1.f)*4.f;
    std::normal_distribution angle_dist(angle_to_player, _angle_variation);
    float const phi = angle_dist(rng);

    // calculate the velocity from the random speed and angle
    return entities.emplace<component::velocity>(munchableid,
            speed*std::cos(phi), speed*std::sin(phi));
}

component::color &
munchable::random_color(entt::registry & entities, entt::entity munchableid,
                        engine_t & rng) const noexcept
{
    // choose a color
    std::uniform_int_distribution<std::size_t> idist(0, _colors.size()-1);
    auto const color = _colors[idist(rng)];

    // add it as a component to the munchable entity
    return entities.emplace<component::color>(munchableid,
            color.r, color.g, color.b);
}
}
