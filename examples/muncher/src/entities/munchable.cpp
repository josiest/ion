#include "entities/munchable.hpp"
#include <entt/entity/registry.hpp>
#include <random>
#include <cmath>

MunchableFactory::MunchableFactory(size_t w, size_t h,
                                   MunchableFactory::engine_t rng)
    : _bounds{0, 0, static_cast<int>(w), static_cast<int>(h)}, _rng{rng}
{
    _colors = std::vector<Color> {
        {0xf3, 0x91, 0x89, 0xff}, {0xbb, 0x80, 0x82, 0xff},
        {0x6e, 0x75, 0x82, 0xff}, {0x04, 0x65, 0x82, 0xff}
    };
}

entt::entity MunchableFactory::make_munchable(entt::registry & registry)
{
    // create the munchable entity
    auto munchable = registry.create();

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
    std::uniform_int_distribution<size_t> pos_dist(0, xvals.size());
    size_t const i = pos_dist(_rng);
    float const x = xvals[i];
    float const y = yvals[i];
    registry.emplace<Position>(munchable, x, y);

    // the distributions for velocity speed and angle
    std::uniform_real_distribution<float> speed_dist(100.f, 200.f);
    float const speed = speed_dist(_rng);

    // compute the angle to the player for the mean distribution
    float angle_to_player = 0.f;
    auto pos_view = registry.view<Position, Player>();
    pos_view.each([&angle_to_player, x, y](auto const & pos) {
        float const dx = pos.x - x;
        float const dy = pos.y - y;
        float const dist = std::sqrt(dx*dx + dy*dy);
        angle_to_player = std::atan2(dx/dist, dy/dist);
    });

    // generate a random angle
    float const pi = std::atan(1.f)*4.f;
    std::normal_distribution<float> angle_dist(angle_to_player, pi/6.f);
    float const phi = angle_dist(_rng);

    // calculate the velocity from the random speed and angle
    registry.emplace<Velocity>(munchable, speed*std::cos(phi),
                                          speed*std::sin(phi));

    // get the size of the player as a mean for the size of the munchable
    int player_size = 0;
    auto size_view = registry.view<Size, Player>();
    size_view.each([&player_size](auto const & size) {
        player_size = size.value;
    });

    // generate a random size
    std::normal_distribution<float> size_dist(player_size, 5.f);
    int const size = std::abs(static_cast<int>(std::round(size_dist(_rng))));
    registry.emplace<Size>(munchable, size);

    // choose a color
    std::uniform_int_distribution<size_t> color_dist(0, _colors.size()-1);
    auto color = _colors[color_dist(_rng)];
    registry.emplace<Color>(munchable, color.r, color.g, color.b, color.a);

    return munchable;
}

void MunchableFactory::filter(entt::registry & registry)
{
    auto view = registry.view<Position, Munchable>();
    view.each([this, &registry](auto const entity, auto & pos) {

        if (pos.x < _bounds.x || pos.x > _bounds.x+_bounds.w
                || pos.y < _bounds.y || pos.y > _bounds.y+_bounds.h) {

            registry.destroy(entity);
        }
    });
}
