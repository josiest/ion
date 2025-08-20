#include "components.hpp"

#include "systems/tile.hpp"
#include "entities/tile.hpp"

#include <ion/ion.hpp>
#include <entt/entt.hpp>
#include <SDL.h>

#include <string>
#include <filesystem>

#include <algorithm>
#include <ranges>

// namespace aliases
namespace cmpt = component;
namespace fs = std::filesystem;
namespace ranges = std::ranges;
namespace views = std::views;

namespace prefab {

tile::tile(std::filesystem::path const & images_path,
           SDL_Color const & static_color, SDL_Color const & placeable_color,
           SDL_Color const & distant_color)

    : _static_color{static_color}, _placeable_color{placeable_color},
      _distant_color{distant_color}, _images_path{images_path}
{
    // full cartesian product of tile names and rotations
    std::unordered_set<cmpt::tile> pairs;
    for (auto name : tileinfo::names) { for (auto rotation : tileinfo::rotations) {
        pairs.emplace(name, rotation);
    }}

    // populate the tiles using the path constructor for an ion surface
    ranges::for_each(pairs, [this](auto const & pair) {
        _tiles.try_emplace(pair, _load_image(pair));
    });
}

entt::entity tile::create(entt::registry & entities,
                          tileinfo::name name, tileinfo::rotation rotation,
                          int x, int y)
{
    auto const tile = entities.create();
    entities.emplace<cmpt::tile>(tile, name, rotation);
    entities.emplace<cmpt::position>(tile, x, y);
    return tile;
}

ion::surface tile::_load_image(cmpt::tile const & tile)
{
    std::stringstream path;
    path << _images_path.string() << "/"
         << tile.name << "-" << tile.rotation << ".bmp";
    return ion::surface::load_bitmap(path.str());
}

}
