#pragma once

#include "types/tile.hpp"
#include "types/point.hpp"
#include "types/components.hpp"
#include "systems/tile.hpp"

#include <ion/graphics/surface.hpp>
#include <entt/entity/registry.hpp>
#include <SDL.h>

#include <filesystem>

namespace prefab {

/** A resource manager for tile surfaces */
class tile {
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
    template<class engine_t>
    entt::entity random_static(entt::registry & entities, pointset & placed_tiles,
                               point auto const & p, engine_t & rng) const
    {
        auto const entity = random_dynamic(entities, rng, p);
        entities.emplace<component::static_tile>(entity);
        placed_tiles.try_emplace(p, entity);
        return entity;
    }

    /** Create a static copy of a tile entity */
    entt::entity static_copy(entt::registry & entities, pointset & placed_tiles,
                             entt::entity tile) const;

    /** Create a tile entity */
    entt::entity create(entt::registry & entities,
                        tiles::Name name, tiles::Rotation rotation,
                        int x, int y) const;

    /** Determine if the tile prefab is not okay to use */
    bool operator!() const;

    /** The error if the tile prefab isn't okay to use */
    std::string error() const;

    /** Get the surface associated with the given pair */
    inline ion::surface & at(tiles::Name name, tiles::Rotation rotation)
    {
        return _tiles.at(std::make_pair(name, rotation));
    }
    inline ion::surface & at(tiles::pair const & tile)
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
    std::unordered_map<tiles::pair, ion::surface> _tiles;

    /** Get the filepath for a tile bitmap image */
    ion::surface _load_image(tiles::pair const & pair);
};
}
