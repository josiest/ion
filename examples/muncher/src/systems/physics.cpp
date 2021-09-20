#include "systems/physics.hpp"
#include "components.hpp"

#include <ion/input.hpp>
#include <SDL2/SDL.h>

#include <cmath>

void accelerate_player(entt::registry & registry, entt::entity player,
                       ion::input::axis2d const & input, float dt)
{
    // do nothing if the player doesn't exist
    if (!registry.valid(player)) {
        return;
    }

    // get the components they need for accelration
    auto & vel = registry.get<Velocity>(player);
    auto const & constants = registry.get<DynamicMover>(player);

    // get the current speed for velocity normalization
    float current_speed = std::sqrt(vel.x*vel.x + vel.y*vel.y);
    Velocity norm_vel = vel;
    if (std::abs(current_speed) > constants.minspeed) {
        norm_vel = Velocity{vel.x/current_speed, vel.y/current_speed};
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

void move_munchies(entt::registry & registry, float dt)
{
    auto view = registry.view<Position, Velocity>();
    view.each([dt](auto & pos, auto const & vel) {
        pos.x += vel.x * dt;
        pos.y -= vel.y * dt;
    });
}

bool collides_with(SDL_Rect const & a, SDL_Rect const & b)
{
    bool const within_width = a.x <= b.x+b.w && a.x+a.w >= b.x;
    bool const within_height = a.y <= b.y+b.h && a.y+a.h >= b.y;
    return within_width && within_height;
}

void devour(entt::registry & registry, entt::entity player)
{
    // do nothing if the player doesn't exist
    if (!registry.valid(player)) {
        return;
    }

    // get the necessary components
    auto [player_pos, player_size_component] =
        registry.get<const Position, Size>(player);

    int const player_size =
        static_cast<int>(std::round(player_size_component.value));

    SDL_Rect const player_box{
        static_cast<int>(std::round(player_pos.x)),
        static_cast<int>(std::round(player_pos.y)),
        player_size, player_size
    };

    // check for collisions against all the munchables
    auto view = registry.view<Position, Size, Munchable>();
    view.each([player, &player_box, &player_size_component, &registry]
              (auto const munchable, auto const & pos, auto const & size) {

        // get the bounding box for the munchable
        int const x = static_cast<int>(std::round(pos.x));
        int const y = static_cast<int>(std::round(pos.y));
        int const s = static_cast<int>(std::round(size.value));
        SDL_Rect const munchable_box{x, y, s, s};

        // do nothing if no collision detected
        if (!collides_with(player_box, munchable_box)) {
            return;
        }

        // destroy the munchable and grow the player if the player's bigger
        if (size.value < player_box.w) {
            player_size_component.value += .05f * size.value;
            registry.destroy(munchable);
        }
        // otherwise, destroy the player
        else {
            registry.destroy(player);
        }
    });
}
