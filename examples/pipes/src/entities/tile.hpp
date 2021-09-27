#pragma once

#include "types/tile.hpp"
#include "types/point.hpp"
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

    /** Create a dynamic tile object */
    entt::entity create(entt::registry & entities,
                        tiles::Name name, tiles::Rotation rotation,
                        point auto const & p)
    { return create(entities, name, rotation, p.x, p.y); }

    entt::entity create(entt::registry & entities,
                        tiles::Name name, tiles::Rotation rotation,
                        int x, int y) const;

    /** Create a static tile object */
    entt::entity create_static(entt::registry & entities,
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
    std::unordered_map<tiles::pair, ion::surface> _tiles;

    /** Get the filepath for a tile bitmap image */
    ion::surface _load_image(tiles::pair const & pair);
};
}
