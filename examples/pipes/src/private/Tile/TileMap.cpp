#include "Pipes/Tile/TileMap.hpp"

#include <ion/konbu.hpp>
#include <SDL2/SDL_surface.h>

#include <sstream>
#include <filesystem>
#include <string_view>

Pipes::TileMap::TileMap(const ion::asset_loader& asset_loader,
                        const std::string_view images_path)
{
    std::stringstream filename;

    const auto root_dir = asset_loader.asset_root_path/images_path;
    if (not std::filesystem::exists(root_dir))
    {
        SDL_Log("Trying to load images at root-directory %s but it doesn't exist\n",
                root_dir.string().c_str());
    }

    for (const auto name : TileInfo::names)
    {
        for (const auto rotation : TileInfo::rotations)
        {
            filename << name << "-" << rotation << ".bmp";
            const auto filepath = root_dir/filename.str();
            if (not std::filesystem::exists(filepath))
            {
                SDL_Log("Expecting tile image at path %s but it doesn't exist!\n", filepath.string().c_str());
            }
            tiles.try_emplace(TileID{ name, rotation }, ion::surface::load_bitmap(filepath));
            filename.str("");
        }
    }
}

SDL_Surface * Pipes::TileMap::image_for(TileInfo::Name name, TileInfo::Rotation rotation) const
{
    if (const auto search = tiles.find(TileID{ name, rotation }); search != tiles.end())
    {
        return search->second;
    }
    return nullptr;
}
