#include "ion/editor/editor.hpp"
#include "ion/engine/sdl_events.hpp"
#include "ion/serialization/paths.hpp"

#include <filesystem>
#include <memory>

#include <SDL3/SDL_log.h>
#include <yaml-cpp/yaml.h>

namespace fs = std::filesystem;
using namespace std::string_literals;
std::string ion::editor_settings::config_path = ""s;

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
    sdl_events::on_quit().connect<&editor::request_quit>(ed.get());
    return std::move(ed);
}

ion::editor_settings ion::editor_settings::load()
{
    if (config_path.empty())
    {
        const fs::path config_dir = paths::config_dir();
        config_path = (config_dir/"editor-settings.yml").string();
    }

    editor_settings settings;
    const auto root = YAML::LoadFile(config_path);
    if (not root) { return settings; }

    const auto general_settings = root["general"];
    if (not general_settings) { return settings; }

    if (const auto editor_name = general_settings["editor-name"])
    {
        settings.window_name = editor_name.Scalar();
    }
    return settings;
}