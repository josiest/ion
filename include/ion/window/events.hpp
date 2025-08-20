#pragma once
#include <entt/signal/sigh.hpp>

namespace ion
{

class event_sink
{
public:
    void poll();

    auto on_quit() { return entt::sink{ quit }; }
    auto on_mouse_scroll() { return entt::sink{ mouse_scroll }; }
    auto on_mouse_up() { return entt::sink{ mouse_up }; }
protected:
    entt::sigh<void()> quit;
    entt::sigh<void(int)> mouse_scroll;
    entt::sigh<void()> mouse_up;
};
}
