#include "entities/tile.hpp"
#include "types/components.hpp"

#include <ion/graphics/surface.hpp>
#include <entt/entity/registry.hpp>
#include <SDL.h>

#include <string>
#include <filesystem>

#include <utility>
#include <algorithm>
#include <numeric>

// namespace aliases
namespace cmpt = component;
namespace fs = std::filesystem;
namespace ranges = std::ranges;
namespace views = std::views;

using namespace std::string_literals;

namespace prefab {

tile::tile(std::filesystem::path const & images_path,
           SDL_Color const & static_color, SDL_Color const & placeable_color,
           SDL_Color const & distant_color)

    : _static_color{static_color}, _placeable_color{placeable_color},
      _distant_color{distant_color}, _images_path{images_path}
{
    // full cartesian product of tile names and rotations
    std::unordered_set<tiles::pair> pairs;
    for (auto name : tiles::names) { for (auto rotation : tiles::rotations) {
        pairs.emplace(name, rotation);
    }}

    // populate the tiles using the path constructor for an ion surface
    auto into_tiles = std::inserter(_tiles, _tiles.end());
    ranges::for_each(pairs, [this](auto const & pair) {
        _tiles.try_emplace(pair, _load_image(pair));
    });
}

entt::entity tile::create(entt::registry & entities,
                          tiles::Name name, tiles::Rotation rotation,
                          int x, int y) const
{
    auto const tile = entities.create();
    entities.emplace<cmpt::tile>(tile, name, rotation);
    entities.emplace<cmpt::position>(tile, x, y);
    return tile;
}

bool tile::operator!() const
{
    // return true if any surface is not okay
    auto tiles = views::values(_tiles);
    return ranges::any_of(tiles, &ion::surface::operator!);
}

std::string tile::error() const
{
    // get the error message of each failed surface
    auto messages = views::values(_tiles)
                  | views::filter(&ion::surface::operator!)
                  | views::transform(&ion::surface::error);

    // and concatenate them into one string with a newline delimeter
    auto string_join = [](std::string && a, std::string const & b) {
        return std::move(a) + "\n" + b;
    };
    return std::accumulate(std::next(messages.begin()), messages.end(),
                           *messages.begin(), string_join);
}

ion::surface tile::_load_image(tiles::pair const & pair)
{
    std::stringstream path;
    path << _images_path.string() << "/"
         << pair.first << "-" << pair.second << ".bmp";
    return ion::surface{path.str()};
}

}
