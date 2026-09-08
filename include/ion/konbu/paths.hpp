#pragma once
#include <filesystem>
#include <string>

#include "ion/engine/engine_component.hpp"

namespace ion
{
struct engine_paths
{
    std::filesystem::path resource_dir() const;

    std::string install_root = "C:/msys64/mingw64";
    std::string resource_relpath = "share/ion";
};

class paths_component : public IEngineComponent
{
public:
    engine_paths paths;
};
}
