#include "ion/konbu/engine_yaml.hpp"

#include <filesystem>

#include "ion/engine.hpp"
#include <yaml-cpp/yaml.h>
#include <SDL3/SDL_log.h>

#include "misc_serialization.hpp"
#include "sdl_yaml.hpp"

ion::project_settings ion::load_project_settings(const YAML::Node & project_config)
{
    project_settings settings;
    if (const YAML::Node name_config = project_config["project-name"];
        name_config.IsDefined() and name_config.IsScalar())
    {
        settings.name = name_config.Scalar();
    }
    else if (name_config.IsDefined())
    {
        const YAML::Exception error(name_config.Mark(), "project-name should be a string");
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s\n", error.what());
    }
    return settings;
}

namespace engine_yaml_helper
{
std::string read_path(const YAML::Node & path_config)
{
    if (not path_config.IsScalar())
    {
        const YAML::Exception error(path_config.Mark(), "path should be a string");
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s\n", error.what());
        return "";
    }
    if (not std::filesystem::exists(path_config.Scalar()))
    {
        const YAML::Exception error(path_config.Mark(), "path doesn't exist");
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s\n", error.what());
        return "";
    }
    return path_config.Scalar();
}
}

ion::engine_settings ion::load_engine_settings(const YAML::Node & engine_config)
{
    engine_settings settings;
    if (const YAML::Node path_config = engine_config["install-dir"]; path_config.IsDefined())
    {
        settings.installation_directory = engine_yaml_helper::read_path(path_config);
    }
    if (const YAML::Node subsystem_config = engine_config["subsystems"]; subsystem_config.IsDefined())
    {
        read_subsystem_flags(subsystem_config, settings.subsystem_flags);
    }
    return settings;
}

ion::window_settings ion::load_window_settings(const YAML::Node & window_config)
{
    window_settings settings;
    if (const YAML::Node name_config = window_config["name"];
        name_config.IsDefined() and name_config.IsScalar())
    {
        settings.name = name_config.Scalar();
    }
    else if (name_config.IsDefined())
    {
        const YAML::Exception error(name_config.Mark(), "window name should be a string");
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s\n", error.what());
    }
    if (const YAML::Node resolution_config = window_config["resolution"];
        resolution_config.IsDefined() and resolution_config.IsScalar())
    {
        read_resolution(resolution_config.Scalar(), settings.width, settings.height);
    }
    else if (resolution_config.IsDefined())
    {
        const YAML::Exception error(resolution_config.Mark(), "resolution format not currently supported");
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s\n", error.what());
    }
    if (const YAML::Node flag_config = window_config["options"];
        flag_config.IsDefined() and flag_config.IsSequence())
    {
        read_window_flags(flag_config, settings.flags);
    }
    else if (flag_config.IsDefined() and flag_config.IsScalar())
    {
        read_window_flag(flag_config.Scalar(), settings.flags);
    }
    else if (flag_config.IsDefined())
    {
        const YAML::Exception error(flag_config.Mark(), "flag format not supported");
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s\n", error.what());
    }
    return settings;
}
