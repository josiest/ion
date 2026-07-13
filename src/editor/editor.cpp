#include "ion/editor/editor.hpp"
#include "ion/engine/sdl_events.hpp"
#include "ion/serialization.hpp"

#include <filesystem>
#include <memory>

#include <SDL3/SDL_log.h>
#include <yaml-cpp/yaml.h>


namespace fs = std::filesystem;
using namespace std::string_literals;
std::string ion::editor_settings::config_path_ = ""s;

std::unique_ptr<ion::editor> ion::editor::initialize()
{
    const auto settings = editor_settings::load();
    auto ed = std::make_unique<editor>();

    ed->sdl = init_sdl(settings.init_flags);
    if (not ed->sdl->was_init())
    {
        SDL_Log("Couldn't initialize SDL: %s\n", SDL_GetError());
        return nullptr;
    }

    ed->window = create_window(
        settings.window_name.c_str(),
        settings.screen_width, settings.screen_height,
        settings.window_flags);
    if (not ed->window)
    {
        SDL_Log("Couldn't create window: %s\n", SDL_GetError());
        return nullptr;
    }

    ed->renderer = create_renderer(ed->window.get());
    if (not ed->renderer)
    {
        SDL_Log("Couldn't create renderer: %s\n", SDL_GetError());
        return nullptr;
    }

    sdl_events::on_quit().connect<&editor::request_quit>(ed.get());
    return std::move(ed);
}

ion::editor_settings ion::editor_settings::load()
{
    editor_settings settings;
    if (not fs::exists(config_path()))
    {
        SDL_Log("Couldn't load settings because of bad path: %s\n", config_path().data());
        return settings;
    }
    const auto root = YAML::LoadFile(std::string{ config_path() });
    if (not root) { return settings; }

    const auto general_settings = root["general"];
    if (not general_settings) { return settings; }

    if (const auto editor_name = general_settings["editor-name"])
    {
        settings.window_name = editor_name.Scalar();
    }
    if (const auto resolution_config = general_settings["resolution"])
    {
        read_resolution(resolution_config.Scalar(), settings.screen_width, settings.screen_height);
    }
    if (const auto subsystem_settings = general_settings["sdl-subsystems"])
    {
        settings.init_flags = 0u;
        read_subsystem_flags(subsystem_settings, settings.init_flags);
    }
    if (const auto window_settings = general_settings["window-options"])
    {
        settings.window_flags = 0u;
        read_window_flags(window_settings, settings.window_flags);
    }
    return settings;
}

YAML::Node ion::editor_settings::load_setting(std::string_view setting_name)
{
    if (not fs::exists(config_path()))
    {
        SDL_Log("Couldn't load settings because of bad path: %s\n", config_path().data());
        return YAML::Node{};
    }
    const auto root = YAML::LoadFile(std::string{ config_path() });
    return root[setting_name];
}

void ion::editor_settings::config_path(std::string_view path)
{
    config_path_ = path;
}

std::string_view ion::editor_settings::config_path()
{
    if (config_path_.empty())
    {
        const fs::path config_dir_ = paths::config_dir();
        config_path_ = (config_dir_/"editor-settings.yml").string();
    }
    return config_path_;
}