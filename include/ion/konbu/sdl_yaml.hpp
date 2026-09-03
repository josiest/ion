#pragma once
#include "ion/containers/lookup_table.hpp"

#include <SDL3/SDL_init.h>

#include <string_view>
#include <yaml-cpp/yaml.h>

namespace YAML
{
class Node;
}

namespace ion
{
bool read_subsystem_flags(const YAML::Node & node, SDL_InitFlags & flags);
bool read_window_flags(const YAML::Node & node, SDL_WindowFlags & flags);

bool read_subsystem_flag(std::string_view src, SDL_InitFlags & flag);
bool read_window_flag(std::string_view src, SDL_WindowFlags & flag);

}