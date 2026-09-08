#include "ion/engine/engine.hpp"
#include "ion/engine/sdl_resources.hpp"
#include "ion/engine/sdl_events.hpp"

#include "ion/konbu/paths.hpp"
#include "ion/konbu/engine_yaml.hpp"

#include <yaml-cpp/yaml.h>
#include <SDL3/SDL_log.h>
#include <filesystem>
#include <SDL3/SDL_filesystem.h>

#include "ion/photon/shader_manager.hpp"

std::unique_ptr<ion::engine> ion::engine::initialize()
{
    namespace fs = std::filesystem;

    engine_settings custom_engine_settings;
    project_settings custom_project_settings;
    window_settings custom_window_settings;

    SDL_Log("Initializing ion with root path: %s\n", SDL_GetBasePath());
    if (const auto settings_path = fs::path(SDL_GetBasePath())/"project.yml";
        fs::exists(settings_path))
    {
        YAML::Node project_config = YAML::LoadFile(settings_path.generic_string());
        custom_project_settings = load_project_settings(project_config);

        if (const YAML::Node engine_config = project_config["engine"];
            engine_config.IsDefined() and engine_config.IsMap())
        {
            custom_engine_settings = load_engine_settings(engine_config);
        }

        if (const YAML::Node window_config = project_config["window"];
            window_config.IsDefined() and window_config.IsMap())
        {
            custom_window_settings = load_window_settings(window_config);
        }
    }
    else
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "Couldn't load project settings because path doesn't exist\n"
                    "  path: %s\n",
                    settings_path.generic_string().c_str());
    }

    auto local_engine = std::make_unique<engine>();
    local_engine->sdl = init_sdl(custom_engine_settings.subsystem_flags);
    if (not local_engine->sdl) { return nullptr; }
    const auto * engine_paths_component = local_engine->emplace_component<paths_component>();

    const bool uses_opengl = (custom_window_settings.flags & SDL_WINDOW_OPENGL) != 0u;
    if (uses_opengl)
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Configuring OpenGL...\n");
        opengl_settings custom_opengl_settings;
        configure_opengl(custom_opengl_settings);
    }

    const std::string window_title = custom_window_settings.name.value_or(custom_project_settings.name);
    const auto * engine_window = local_engine
        ->emplace_component<window_component>(window_title,
                                              custom_window_settings.width, custom_window_settings.height,
                                              custom_window_settings.flags);
    if (not engine_window->get()) { return nullptr; }

    if (uses_opengl)
    {
        const auto * engine_gl_ctx = local_engine ->emplace_component<opengl_component>(engine_window->get());
        if (not engine_gl_ctx->gl_context) { return nullptr; }
        local_engine->emplace_component<shader_manager_component>(engine_paths_component->paths);
    }
    else
    {
        const auto * engine_renderer = local_engine ->emplace_component<renderer_component>(engine_window->get());
        if (not engine_renderer->renderer) { return nullptr; }
    }

    GEngine = local_engine.get();
    sdl_events::on_quit().connect<&engine::quit>(local_engine.get());
    return local_engine;
}

void ion::engine::start()
{
    for (const auto & component : components)
    {
        component->start();
    }
}

void ion::engine::quit()
{
    wants_to_quit = true;
    components.reverse();
    for (const auto & component : components)
    {
        component->quit();
    }
}