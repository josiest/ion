#pragma once
#include <entt/signal/sigh.hpp>

namespace ion
{

class event_sink
{
public:
    void poll();

    inline auto on_quit() { return entt::sink{ quit }; }
protected:
    entt::sigh<void()> quit;
};
}
