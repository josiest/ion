#include "ion/konbu/paths.hpp"
#include <filesystem>

namespace fs = std::filesystem;
std::filesystem::path ion::engine_paths::resource_dir() const
{
    return fs::path(install_root)/resource_relpath;
}
