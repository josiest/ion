#include "ion/serialization/paths.hpp"

#include <filesystem>
#include <SDL3/SDL_filesystem.h>

namespace fs = std::filesystem;
std::string ion::paths::root_dir()
{
    const fs::path base_path = SDL_GetBasePath();
    return base_path.parent_path().string();
}

std::string ion::paths::config_dir()
{
    const fs::path root_path = root_dir();
    return (root_path/"config").string();
}