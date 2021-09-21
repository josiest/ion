#include "systems/physics.hpp"
#include "components.hpp"

#include <ion/input.hpp>
#include <SDL2/SDL.h>

#include <cmath>

namespace systems {

namespace cmpt = component;
void accelerate_player(entt::registry & entities, entt::entity player,
                       ion::input::axis2d const & input, float dt)
{
    // do nothing if the player doesn't exist
    //      or doesn't have the right components
    if (not entities.valid(player) ||
            not entities.all_of<cmpt::bbox, cmpt::dynamic_mover>(player)) {
        return;
    }

    // get the components they need for accelration
    auto [v, constants] =
        entities.get<cmpt::velocity, cmpt::dynamic_mover const>(player);

    // calculate the acceleration from input and from friction
    float const acc_in = constants.acceleration * dt;
    float const acc_fr = -constants.friction * dt;

    // calculate the velocity from the various accelerations
    float const eps = constants.minspeed;
    v += acc_in*normalized({input.x(), input.y()}, eps) + acc_fr*normalized(v, eps);

    // clamp the velocity to be within the defined thresholds
    float const speed = std::min(magnitude(v), constants.maxspeed);
    v = speed * normalized(v, eps);
}

void move_munchies(entt::registry & entities, float dt)
{
    auto munchies = entities.view<cmpt::bbox, cmpt::velocity const>();
    munchies.each([dt](auto & p, auto const & v) {
        p.x += v.x * dt;
        p.y -= v.y * dt;
    });
}

bool collides_with(cmpt::bbox const & a, cmpt::bbox const & b)
{
    bool const within_width = a.x <= b.x+b.size and a.x+a.size >= b.x;
    bool const within_height = a.y <= b.y+b.size and a.y+a.size >= b.y;
    return within_width and within_height;
}

cmpt::velocity normalized(cmpt::velocity const & v, float eps)
{

    float const speed = magnitude(v);

    // return the zero vector if speed is below threshold
    if (speed < eps) { return {0.f, 0.f}; }

    // otherwise return the normalized velocity
    return {v.x/speed, v.y/speed};
}

cmpt::velocity operator+(cmpt::velocity const & lhs,
                         cmpt::velocity const & rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

cmpt::velocity & operator+=(cmpt::velocity & lhs,
                            cmpt::velocity const & rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

cmpt::velocity operator*(float lhs, cmpt::velocity const & rhs)
{
    return {lhs*rhs.x, lhs*rhs.y};
}

float magnitude(cmpt::velocity const & v)
{
    return std::sqrt(v.x*v.x + v.y*v.y);
}
}
