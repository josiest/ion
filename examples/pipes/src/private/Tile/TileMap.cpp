#include "Pipes/Tile/TileMap.hpp"

#include <sstream>
#include <filesystem>
#include <string_view>

#include <SDL3/SDL_log.h>
#include <ion/serialization/paths.hpp>

#include "ion/editor/editor.hpp"

namespace fs = std::filesystem;

Pipes::TileMap::TileMap(const std::string_view images_path)
{
    std::stringstream filename;

    const fs::path root_dir = ion::paths::root_dir();
    const auto images_dir = root_dir/"resources"/images_path;

    if (not std::filesystem::exists(images_dir))
    {
        SDL_Log("Trying to load images at root-directory %s but it doesn't exist\n",
                images_dir.string().c_str());
    }

    for (const auto name : TileInfo::names)
    {
        for (const auto rotation : TileInfo::rotations)
        {
            filename << name << "-" << rotation << ".bmp";
            const auto filepath = images_dir/filename.str();
            if (not std::filesystem::exists(filepath))
            {
                SDL_Log("Expecting tile image at path %s but it doesn't exist!\n", filepath.string().c_str());
            }
            tiles.try_emplace(TileID{ name, rotation }, ion::load_bitmap(filepath.string()));
            filename.str("");
        }
    }
}

SDL_Surface * Pipes::TileMap::image_for(TileInfo::Name name, TileInfo::Rotation rotation) const
{
    if (const auto search = tiles.find(TileID{ name, rotation }); search != tiles.end())
    {
        return search->second.get();
    }
    return nullptr;
}
