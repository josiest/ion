#pragma once
#include <entt/signal/sigh.hpp>
#include <SDL3/SDL_events.h>

namespace ion
{
class sdl_events
{
public:
    static void poll();
    static auto on_poll() { return entt::sink{ poll_signal }; }
    static auto on_quit() { return entt::sink{ quit_signal }; }
private:
    static entt::sigh<void(SDL_Event*)> poll_signal;
    static entt::sigh<void()> quit_signal;
};
}
