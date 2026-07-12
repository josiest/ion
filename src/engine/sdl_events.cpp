#include <ion/engine/sdl_events.hpp>
#include <SDL3/SDL_events.h>

entt::sigh<void(SDL_Event*)> ion::sdl_events::poll_signal{};
entt::sigh<void()> ion::sdl_events::quit_signal{};

void ion::sdl_events::poll()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        poll_signal.publish(&event);
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            quit_signal.publish();
            break;
        default:
            break;
        }
    }
}