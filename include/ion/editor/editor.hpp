#pragma once
#include <ion/engine/sdl_resources.hpp>
#include <SDL3/SDL_init.h>

inline namespace ion
{
class editor
{
public:
    static std::unique_ptr<editor> initialize();
    editor() = default;

    bool has_quit() const { return wants_to_quit; }
    void request_quit() { wants_to_quit = true; }

    sdl_window window = nullptr;
    sdl_renderer renderer = nullptr;
protected:
    sdl_system sdl = nullptr;
    bool wants_to_quit = false;
};

struct editor_settings
{
    static std::string config_path;
    static editor_settings load();

    SDL_InitFlags init_flags = SDL_INIT_VIDEO;

    std::string window_name = "ion Editor";
    std::uint32_t screen_width = 1920;
    std::uint32_t screen_height = 1080;
    SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE;
};
}
