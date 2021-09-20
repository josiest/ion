#include "systems/physics.hpp"
#include "components.hpp"

#include <ion/input.hpp>
#include <SDL2/SDL.h>

#include <cmath>

namespace systems {

void accelerate_player(entt::registry & entities, entt::entity player,
                       ion::input::axis2d const & input, float dt)
{
    // do nothing if the player doesn't exist
    //      or doesn't have the right components
    if (not entities.valid(player) ||
            not entities.all_of<component::bbox,
                                component::dynamic_mover>(player)) {
        return;
    }

    // get the components they need for accelration
    auto [vel, constants] = entities.get<component::velocity,
                                         component::dynamic_mover const>(player);

    // get the current speed for velocity normalization
    float current_speed = std::sqrt(vel.x*vel.x + vel.y*vel.y);
    component::velocity norm_vel = vel;
    if (std::abs(current_speed) > constants.minspeed) {
        norm_vel = {vel.x/current_speed, vel.y/current_speed};
    }

    // calculate the acceleration from input and from friction
    float const acc_in = constants.acceleration * dt;
    float const acc_fr = -constants.friction * dt;

    // normalize the input
    float inx = input.x();
    float iny = input.y();
    float const in_norm = std::sqrt(inx*inx + iny*iny);
    if (std::abs(in_norm) > 0.f) {
        inx = inx/in_norm;
        iny = iny/in_norm;
    }

    // calculate the velocity from the various accelerations
    vel.x += acc_in*inx + acc_fr*norm_vel.x;
    vel.y += acc_in*iny + acc_fr*norm_vel.y;

    // cut the velocity if below threshold
    current_speed = std::sqrt(vel.x*vel.x + vel.y*vel.y);
    if (std::abs(current_speed) <= constants.minspeed) {
        vel.x = 0.f;
        vel.y = 0.f;
    }
    // clamp the velocity if above max speed
    else if (current_speed > constants.maxspeed) {
        vel.x = constants.maxspeed * vel.x/current_speed;
        vel.y = constants.maxspeed * vel.y/current_speed;
    }
}

void move_munchies(entt::registry & entities, float dt)
{
    auto munchies = entities.view<component::bbox,
                                  component::velocity const>();

    munchies.each([dt](auto & p, auto const & v) {
        p.x += v.x * dt;
        p.y -= v.y * dt;
    });
}

bool collides_with(component::bbox const & a, component::bbox const & b)
{
    bool const within_width = a.x <= b.x+b.size and a.x+a.size >= b.x;
    bool const within_height = a.y <= b.y+b.size and a.y+a.size >= b.y;
    return within_width and within_height;
}
}
