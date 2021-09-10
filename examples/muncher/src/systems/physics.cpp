#include "systems/physics.hpp"
#include "components.hpp"
#include <ion/input.hpp>

void accelerate_player(entt::registry & registry,
                       ion::input::InputAxis2D const & input, float dt)
{
    auto view = registry.view<Velocity, DynamicMover>();
    view.each([&input, dt](auto & vel, auto const & constants) {

        float current_speed = std::sqrt(vel.x*vel.x + vel.y*vel.y);
        Velocity norm_vel = vel;
        if (std::abs(current_speed) > constants.minspeed) {
            norm_vel = Velocity{vel.x/current_speed, vel.y/current_speed};
        }

        float const acc_in = constants.acceleration * dt;
        float const acc_fr = -constants.friction * dt;

        vel.x += acc_in*input.x() + acc_fr*norm_vel.x;
        vel.y += acc_in*input.y() + acc_fr*norm_vel.y;

        current_speed = std::sqrt(vel.x*vel.x + vel.y*vel.y);
        if (std::abs(current_speed) <= constants.minspeed) {
            vel.x = 0.f;
            vel.y = 0.f;
        }
        else if (current_speed > constants.maxspeed) {
            vel.x = constants.maxspeed * vel.x/current_speed;
            vel.y = constants.maxspeed * vel.y/current_speed;
        }
    });
}

void move_munchies(entt::registry & registry, float dt)
{
    auto view = registry.view<Position, Velocity>();
    view.each([dt](auto & pos, auto const & vel) {
        pos.x += vel.x * dt;
        pos.y -= vel.y * dt;
    });
}
