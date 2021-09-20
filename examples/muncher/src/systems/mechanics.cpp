#include "systems/physics.hpp"
#include "systems/mechanics.hpp"
#include "components.hpp"

#include <entt/entity/registry.hpp>
#include <cstdint>

#include <algorithm>
#include <vector>

namespace ranges = std::ranges;
namespace systems {

void munch(entt::registry & entities, entt::entity player)
{
    // do nothing if the player doesn't exist
    if (not entities.valid(player) ||
            not entities.all_of<component::bbox>(player)) {
        return;
    }

    // get the necessary components
    auto & player_box = entities.get<component::bbox>(player);

    // check for collisions against all the munchables
    auto munchables = entities.view<component::bbox, component::munchable>();
    munchables.each([player, &player_box, &entities]
                    (auto const munchable, auto const & munchable_box) {

        // do nothing if no collision detected
        if (not collides_with(player_box, munchable_box)) {
            return;
        }

        // destroy the munchable and grow the player if the player's bigger
        if (munchable_box.size < player_box.size) {
            player_box.size += .05f * munchable_box.size;
            entities.destroy(munchable);
        }
        // otherwise, destroy the player if they still exist
        else if (entities.valid(player)) {
            entities.destroy(player);
        }
    });
}

void filter_munchables(entt::registry & entities,
                       std::uint32_t width, std::uint32_t height)
{
    // view the position component of all munchable entities
    auto munchables = entities.view<component::bbox const, component::munchable>();

    // determine if a munchable entity is out of bounds
    auto out_of_bounds = [&munchables, width, height] (auto const munchable) {
        auto const & [p] = munchables.get(munchable);
        return p.x+p.size < 0 || p.x > width || p.y+p.size < 0 || p.y > height;
    };

    // filter out any munchables that are within bounds
    std::vector<entt::entity> lost_munchables;
    ranges::copy_if(munchables, std::back_inserter(lost_munchables), out_of_bounds);

    // erase the munchables that are out of bounds
    entities.destroy(lost_munchables.begin(), lost_munchables.end());
}
}
