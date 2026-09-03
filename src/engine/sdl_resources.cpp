#include "ion/engine/sdl_resources.hpp"

#include <SDl3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>

#include <string_view>
#include <cstdint>
#include <cstdlib>

ion::internal::sdl_lifetime_helper::sdl_lifetime_helper(std::uint32_t subsystem_flags)
{
    for (const auto & [flag, name] : subsystem_flag_names)
    {
        if (const SDL_WindowFlags mask = flag & subsystem_flags; mask != 0u)
        {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Initializing SDL %s subsystem...\n", name.data());
        }
    }
    if (not SDL_Init(subsystem_flags))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
    }
}

ion::internal::sdl_system_DEPRECATED::sdl_system_DEPRECATED(std::uint32_t init_flags)
    : was_init_(SDL_Init(init_flags))
{
    for (const auto & [flag, name] : subsystem_flag_names)
    {
        if (const SDL_WindowFlags mask = flag & init_flags; mask == 0u) { continue; }
        std::string_view subsystem_status_message = SDL_WasInit(flag)? "SUCCESS" : "FAILURE";
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Initializing SDL %s subsystem... %s\n",
                                                  name.data(), subsystem_status_message.data());
    }
    if (not was_init_)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
    }
}

void ion::internal::sdl_deleter::operator()(const sdl_lifetime_helper * ctx) const
{

    if (constexpr std::uint32_t ANY_SUBSYSTEM = 0u; SDL_WasInit(ANY_SUBSYSTEM))
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Quitting SDL...\n");
        SDL_Quit();
    }
}

void ion::internal::sdl_deleter::operator()(const sdl_system_DEPRECATED * sdl) const
{
    if (sdl->was_init())
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Quitting SDL...\n");
        SDL_Quit();
    }
    else
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Trying to quit SDL but it wasn't initialized!\n");
    }
}

void ion::internal::sdl_deleter::operator()(SDL_Window * window) const
{
    if (not window) { return; }
    if (const std::string_view window_title = SDL_GetWindowTitle(window); not window_title.empty())
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Destroying window \"%s\"...\n", window_title.data());
    }
    else if (const int window_id = SDL_GetWindowID(window))
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Destroying window (id %d)...\n", window_id);
    }
    else
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Trying to destroy window with no id! %s\n", SDL_GetError());
    }
    SDL_DestroyWindow(window);
}

void ion::internal::sdl_deleter::operator()(SDL_Renderer * renderer) const
{
    if (not renderer) { return; }
    const auto prop_id = SDL_GetRendererProperties(renderer);

    if (auto * window = static_cast<SDL_Window *>
        (
            SDL_GetPointerProperty(prop_id, SDL_PROP_RENDERER_WINDOW_POINTER, nullptr)
        ))
    {
        if (const std::string_view window_title = SDL_GetWindowTitle(window); not window_title.empty())
        {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Destroying renderer for window \"%s\"...\n",
                                                      window_title.data());
        }
        else if (const SDL_WindowID window_id = SDL_GetWindowID(window))
        {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Destroying renderer for window (id %d)...\n", window_id);
        }
        else
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Trying to destroy renderer for a window with no id! %s\n",
                                                       SDL_GetError());
        }
    }
    else
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Trying to destroy renderer but it has no window!\n");
    }
    SDL_DestroyRenderer(renderer);
}

void ion::internal::sdl_deleter::operator()(SDL_Surface * surface) const
{
    if (surface) { SDL_DestroySurface(surface); }
}

ion::sdl_system_DEPRECATED ion::init_sdl_DEPRECATED(std::uint32_t init_flags)
{
    return sdl_system_DEPRECATED(new internal::sdl_system_DEPRECATED(init_flags), internal::sdl_deleter{});
}

ion::sdl_context ion::init_sdl(std::uint32_t subsystem_flags)
{
    return sdl_context(new internal::sdl_lifetime_helper(subsystem_flags), internal::sdl_deleter{});
}

ion::sdl_window ion::create_window(std::string_view name, int width, int height, std::uint32_t window_flags)
{
    SDL_Window * window = SDL_CreateWindow(name.data(), width, height, window_flags);
    const std::string_view success_message = window? "SUCCESS" : "FAILURE";
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Creating window \"%s\"... %s\n",
                name.data(), success_message.data());

    return sdl_window(window, internal::sdl_deleter{});
}

ion::sdl_renderer ion::create_renderer(SDL_Window * window)
{
    SDL_Renderer * renderer = SDL_CreateRenderer(window, nullptr);
    const std::string_view success_message = renderer? "SUCCESS" : "FAILURE";

    if (const std::string_view window_title = SDL_GetWindowTitle(window); not window_title.empty())
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Creating SDL hardware renderer for window \"%s\"... %s\n",
                    window_title.data(), success_message.data());
    }
    else if (const SDL_WindowID window_id = SDL_GetWindowID(window))
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Creating SDL hardware renderer for window (id %d)...\n", window_id);
    }
    else
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Trying to create renderer for a window with no id! %s\n",
                                                   SDL_GetError());
    }
    return sdl_renderer(renderer, {});
}

ion::sdl_surface ion::load_bitmap(std::string_view path)
{
    return { SDL_LoadBMP(path.data()), {} };
}
