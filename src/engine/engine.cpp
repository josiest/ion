#include "ion/engine/engine.hpp"
#include "ion/engine/sdl_resources.hpp"
#include "ion/engine/sdl_events.hpp"

std::unique_ptr<ion::engine> ion::engine::initialize()
{
    project_settings project_config;
    engine_settings engine_config;
    window_settings window_config;

    auto local_engine = std::make_unique<engine>();
    local_engine->sdl = init_sdl(engine_config.subsystem_flags);
    if (not local_engine->sdl) { return nullptr; }

    const std::string_view window_title = window_config.name.value_or(project_config.name);
    auto * window_reference = local_engine
        ->emplace_component<window_component>(window_title, window_config.width, window_config.height,
                                              window_config.flags);
    if (not window_reference->window) { return nullptr; }

    auto * renderer_reference = local_engine
        ->emplace_component<renderer_component>(window_reference->window.get());
    if (not renderer_reference->renderer) { return nullptr; }

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