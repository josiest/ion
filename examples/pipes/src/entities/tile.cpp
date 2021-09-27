#include "entities/tile.hpp"
#include "types/components.hpp"
#include <SDL2/SDL_surface.h>
#include <entt/entity/registry.hpp>

namespace tiles {

entt::entity make(entt::registry & entities,
                  Name name, Rotation rotation, int x, int y)
{
    auto const tile = entities.create();
    entities.emplace<component::tile>(tile, name, rotation);
    entities.emplace<component::position>(tile, x, y);
    return tile;
}

entt::entity make(entt::registry & entities,
                  Name name, Rotation rotation, SDL_Point const & p)
{
    return make(entities, name, rotation, p.x, p.y);
}

entt::entity make_static(entt::registry & entities,
                         Name tilename, Rotation rotation, int x, int y)
{
    auto const tile = make(entities, tilename, rotation, x, y);
    entities.emplace<component::static_tile>(tile);
    return tile;
}

}
