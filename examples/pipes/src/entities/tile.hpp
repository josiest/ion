#pragma once

#include "components.hpp"
#include "systems/point.hpp"
#include "systems/tile.hpp"

#include <ion/ion.hpp>
#include <entt/entt.hpp>
#include <SDL.h>

#include <filesystem>

namespace prefab {

namespace tileinfo = component::tileinfo;

/** A resource manager for tile surfaces */
class tile : public ion::isotope {
public:
    /** Create a tile prefab with the specified colors */
    tile(std::filesystem::path const & images_path,
         SDL_Color const & static_color, SDL_Color const & placable_color,
         SDL_Color const & distant_color);

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
    entt::entity create(entt::registry & entities,
                        tileinfo::name name, tileinfo::rotation rotation,
                        int x, int y) const;

    /** Get the surface associated with the given tile */
    inline ion::surface & at(component::tile const & tile)
    {
        return _tiles.at(tile);
    }

    /** The background color for static tiles */
    inline SDL_Color const & static_color() const { return _static_color; }
    /** The background color for placeable tiles */
    inline SDL_Color const & placeable_color() const { return _placeable_color; }
    /** The background color for distant tiles */
    inline SDL_Color const & distant_color() const { return _distant_color; }
private:
    SDL_Color _static_color, _placeable_color, _distant_color;
    std::filesystem::path _images_path;

    // the loaded tile surfaces
    std::unordered_map<component::tile, ion::surface> _tiles;

    /** Get the filepath for a tile bitmap image */
    ion::surface _load_image(component::tile const & tile);
};
}
