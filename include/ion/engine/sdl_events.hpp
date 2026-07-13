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
    static auto on_mouse_scroll() { return entt::sink{ mouse_scroll_signal }; }
    static auto on_mouse_up() { return entt::sink{ mouse_up_signal }; }
    static auto on_mouse_moved() { return entt::sink{ mouse_moved_signal }; }
private:
    static entt::sigh<void(SDL_Event*)> poll_signal;
    static entt::sigh<void()> quit_signal;
    static entt::sigh<void(int)> mouse_scroll_signal;
    static entt::sigh<void()> mouse_up_signal;
    static entt::sigh<void(int, int)> mouse_moved_signal;
};
}
