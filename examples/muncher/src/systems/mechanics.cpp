#include "systems/physics.hpp"
#include "systems/mechanics.hpp"
#include "components.hpp"

#include <entt/entity/registry.hpp>
#include <cstdint>

#include <algorithm>
#include <vector>

namespace ranges = std::ranges;
namespace systems {

// determine if a munchable collides with the player
decltype(auto) collides_with_player(auto & player_box, auto & munchables)
{
    return [&player_box, &munchables](auto const munchable) {
        auto const & [munchable_box] = munchables.get(munchable);
        return collides_with(player_box, munchable_box);
    };
}

// determine if a munchable is bigger than the player
decltype(auto) bigger_than_player(auto & player_box, auto & munchables)
{
    return [&player_box, &munchables](auto const munchable) {
        auto const & [munchable_box] = munchables.get(munchable);
        return munchable_box.size >= player_box.size;
    };
}

// grow the player's bounding box
decltype(auto) grow_player_by(auto & player_box, auto growth, auto & munchables)
{
    return [&player_box, growth, &munchables](auto const munchable) {
        auto const & [munchable_box] = munchables.get(munchable);
        player_box.size += growth*munchable_box.size;
    };
}

void munch(entt::registry & entities, entt::entity player)
{
    // do nothing if the player doesn't exist
    if (not entities.valid(player) ||
            not entities.all_of<component::bbox>(player)) {
        return;
    }
    // get the required components of the player
    auto & player_box = entities.get<component::bbox>(player);

    // view all bounding box components of the munchables
    auto munchables = entities.view<component::bbox, component::munchable>();

    // define the mechanic systems
    auto are_colliding = collides_with_player(player_box, munchables);
    auto are_bigger = bigger_than_player(player_box, munchables);
    auto grow_player = grow_player_by(player_box, .05f, munchables);

    // filter out all munchables not colliding with the player
    std::vector<entt::entity> colliding_munchables;
    auto it = std::back_inserter(colliding_munchables);
    ranges::copy_if(munchables, it, are_colliding);

    // if any munchables are bigger than the player, munch the player
    if (ranges::any_of(colliding_munchables, are_bigger)) {
        entities.destroy(player);
    }
    // otherwise munch all colliding munchables
    else {
        ranges::for_each(colliding_munchables, grow_player);
        entities.destroy(colliding_munchables.begin(),
                         colliding_munchables.end());
    }
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
