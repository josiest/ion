#include "ion/serialization/sdl_yaml.hpp"
#include <string_view>

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>

#include <yaml-cpp/yaml.h>

bool ion::read_subsystem_flags(const YAML::Node & node, SDL_InitFlags & flags)
{
    if (not node.IsSequence()) { return false; }
    bool any_success = false;
    for (const auto & flag_config : node)
    {
        SDL_InitFlags flag = 0u;
        if (read_subsystem_flag(flag_config.Scalar(), flag))
        {
            any_success = true;
        }
        else
        {
            SDL_Log("Encountered bad SDL subsystem \"%s\" in editor settings",
                    flag_config.Scalar().c_str());
        }
        flags |= flag;
    }
    return any_success;
}

bool ion::read_window_flags(const YAML::Node & node, SDL_WindowFlags & flags)
{
    if (not node.IsSequence()) { return false; }
    bool any_success = false;
    for (const auto & flag_config : node)
    {
        SDL_WindowFlags flag = 0u;
        if (read_window_flag(flag_config.Scalar(), flag))
        {
            any_success = true;
        }
        else
        {
            SDL_Log("Encountered bad window option \"%s\" in editor settings",
                    flag_config.Scalar().c_str());
        }
        flags |= flag;
    }
    return any_success;
}

bool ion::read_subsystem_flag(std::string_view src, SDL_InitFlags & flag)
{
    if (auto * result = internal::subsystem_flags.find(src))
    {
        flag = result->first;
        return true;
    }
    return false;
}

bool ion::read_window_flag(std::string_view src, SDL_WindowFlags & flag)
{
    if (auto * result = internal::window_flags.find(src))
    {
        flag = result->first;
        return true;
    }
    return false;
}
