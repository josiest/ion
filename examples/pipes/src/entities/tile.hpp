#pragma once

#include "components.hpp"
#include "systems/point.hpp"
#include "systems/tile.hpp"

#include <ion/ion.hpp>
#include <entt/entt.hpp>

#include <SDL2/SDL_pixels.h>
#include <filesystem>

namespace Pipes
{
struct TileSettings
{
    SDL_Color static_color{ 0x42, 0x87, 0xf5, 0xff };
    SDL_Color placeable_color{ 0x9d, 0xbe, 0xf5, 0xff };
    SDL_Color distant_color{ 0xd3, 0xd3, 0xd3, 0xff };
};
}

namespace ion
{
template<>
inline auto reflect<Pipes::TileSettings>()
{
    using namespace entt::literals;
    return entt::meta_factory<Pipes::TileSettings>{}
        .type("Pipes::TileSettings"_hs)
        .data<&Pipes::TileSettings::static_color>("static-color"_hs)
        .data<&Pipes::TileSettings::placeable_color>("placeable-color"_hs)
        .data<&Pipes::TileSettings::distant_color>("distant-color"_hs);
}
}

namespace Pipes
{
class TileMap
{
public:
    static TileMap load_all(const ion::asset_loader& asset_loader, std::string_view images_path);
    SDL_Surface * image_for(const component::tile& tile);
private:
    TileMap() = default;
    std::unordered_map<component::tile, ion::surface> tiles;
};
}

namespace prefab {

namespace tileinfo = component::tileinfo;

/** A resource manager for tile surfaces */
class tile : public ion::isotope {
public:
    /** Create a tile prefab with the specified colors */
    explicit tile(std::string_view images_path);

    /** Create a dynamic random tile object */
    template<class engine_t>
    entt::entity random_dynamic(entt::registry & entities, engine_t & rng,
                                point auto const & p) const
    {
        return create(entities, systems::random_name(rng),
                                systems::random_rotation(rng), p.x, p.y);
    }

    /** Create a static random tile object */
    template<class engine_t, point point_t>
    entt::entity random_static(entt::registry & entities,
                               pointset & placed_tiles,
                               point_t const & p, engine_t & rng) const
    {
        auto const entity = random_dynamic(entities, rng, p);
        entities.emplace<component::static_tile>(entity);
        placed_tiles.try_emplace(p, entity);
        return entity;
    }

    /** Create a static copy of a tile entity */
    template<class engine_t>
    entt::entity static_copy(entt::registry & entities, pointset & placed_tiles,
                             entt::entity mouse_tile, engine_t & rng) const
    {
        // get the required components
        auto const & [info, p] = entities.get<component::tile,
                                              component::position>(mouse_tile);

        // return the existing entity if it already has been placed
        auto const search = placed_tiles.find(p);
        if (search != placed_tiles.end()) {
            return search->second;
        }
        // return mouse entity if mouse is too far
        else if (not systems::is_adjacent(placed_tiles, p)) {
            return mouse_tile;
        }
        // otherwise create a copy of the tile
        auto const entity = create(entities, info.name, info.rotation, p.x, p.y);
        entities.emplace<component::static_tile>(entity);

        // and mark it as placed
        placed_tiles.try_emplace(search, p, entity);

        // then randomize the mouse tile
        systems::randomize_tile(entities, mouse_tile, rng);
        return entity;
    }

    /** Create a tile entity */
    static entt::entity create(entt::registry & entities,
                               tileinfo::name name, tileinfo::rotation rotation,
                               int x, int y);

    /** The background color for static tiles */
    inline SDL_Color const & static_color() const { return settings.static_color; }
    /** The background color for placeable tiles */
    inline SDL_Color const & placeable_color() const { return settings.placeable_color; }
    /** The background color for distant tiles */
    inline SDL_Color const & distant_color() const { return settings.distant_color; }

    Pipes::TileMap loaded_tiles;
private:
    Pipes::TileSettings settings;
};
}
