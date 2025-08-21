#include "components.hpp"

#include "systems/tile.hpp"
#include "entities/tile.hpp"

#include <ion/ion.hpp>
#include <entt/entt.hpp>

#include <filesystem>
#include <ranges>

// namespace aliases
namespace cmpt = component;
namespace fs = std::filesystem;
namespace ranges = std::ranges;
namespace views = std::views;

namespace prefab {

tile::tile(std::string_view images_path)

    : loaded_tiles{ Pipes::TileMap::load_all(ion::asset_loader{}, images_path) }
{
}

entt::entity tile::create(entt::registry & entities,
                          Pipes::TileInfo::Name name,
                          Pipes::TileInfo::Rotation rotation,
                          int x, int y,
                          const SDL_Color& color)
{
    auto const tile = entities.create();
    entities.emplace<Pipes::Component::Tile>(tile, name, rotation, color);
    entities.emplace<cmpt::position>(tile, x, y);
    return tile;
}
}
