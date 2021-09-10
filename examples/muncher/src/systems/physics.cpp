#include "systems/physics.hpp"
#include "components.hpp"
#include <ion/input.hpp>

void accelerate_player(entt::registry & registry,
                       ion::input::InputAxis2D const & axis, float dt)
{
    auto view = registry.view<Velocity, DynamicMover>();
    view.each([&axis, dt](auto & vel, auto const & constants) {

        vel.x = axis.x();
        vel.y = axis.y();

        float const current_speed = std::sqrt(vel.x*vel.x + vel.y*vel.y);
        if (current_speed != 0) {
            vel.x = constants.speed * vel.x/current_speed;
            vel.y = constants.speed * vel.y/current_speed;
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
