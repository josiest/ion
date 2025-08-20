#include "components.hpp"

#include "systems/tile.hpp"
#include "entities/tile.hpp"

#include <ion/ion.hpp>
#include <entt/entt.hpp>

#include <string>
#include <filesystem>
#include <sstream>

#include <ranges>

// namespace aliases
namespace cmpt = component;
namespace fs = std::filesystem;
namespace ranges = std::ranges;
namespace views = std::views;

Pipes::TileMap Pipes::TileMap::load_all(const ion::asset_loader& asset_loader,
                                        const std::string_view images_path)
{
    TileMap tilemap;
    std::stringstream filename;

    const auto root_dir = asset_loader.asset_root_path/images_path;
    if (not fs::exists(root_dir))
    {
        SDL_Log("Trying to load images at root-directory %s but it doesn't exist\n",
                root_dir.string().c_str());
    }

    for (const auto name : component::tileinfo::names)
    {
        for (const auto rotation : component::tileinfo::rotations)
        {
            filename << name << "-" << rotation << ".bmp";
            const auto filepath = root_dir/filename.str();
            if (not fs::exists(filepath))
            {
                SDL_Log("Expecting tile image at path %s but it doesn't exist!\n", filepath.string().c_str());
            }
            tilemap.tiles.try_emplace(component::tile{ name, rotation },
                                      ion::surface::load_bitmap(filepath));
            filename.str("");
        }
    }
    return tilemap;
}

SDL_Surface * Pipes::TileMap::image_for(const component::tile& tile)
{
    if (const auto search = tiles.find(tile); search != tiles.end())
    {
        return search->second;
    }
    return nullptr;
}

namespace prefab {

tile::tile(std::string_view images_path)

    : loaded_tiles{ Pipes::TileMap::load_all(ion::asset_loader{}, images_path) }
{
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
}
