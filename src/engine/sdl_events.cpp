#include <ion/engine/sdl_events.hpp>
#include <SDL3/SDL_events.h>

entt::sigh<void(SDL_Event*)> ion::sdl_events::poll_signal{};
entt::sigh<void()> ion::sdl_events::quit_signal{};
entt::sigh<void(int)> ion::sdl_events::mouse_scroll_signal{};
entt::sigh<void()> ion::sdl_events::mouse_up_signal{};
entt::sigh<void(int, int)> ion::sdl_events::mouse_moved_signal{};

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

        case SDL_EVENT_MOUSE_WHEEL:
            mouse_scroll_signal.publish(event.wheel.y);
            break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
            mouse_up_signal.publish();
            break;

        case SDL_EVENT_MOUSE_MOTION:
            mouse_moved_signal.publish(event.motion.x, event.motion.y);
            break;

        default:
            break;
        }
    }
}