#include <ion/editor/editor.hpp>
#include <ion/engine/sdl_events.hpp>
#include <memory>
#include <SDL3/SDL_log.h>

std::unique_ptr<ion::editor> ion::editor::initialize()
{
    const editor_settings settings;
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
