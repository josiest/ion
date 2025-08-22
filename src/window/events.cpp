#include "ion/window/events.hpp"
#include <SDL2/SDL_events.h>

void ion::event_sink::poll()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            quit.publish();
            break;

        case SDL_MOUSEWHEEL:
            mouse_scroll.publish(event.wheel.y);
            break;

        case SDL_MOUSEBUTTONUP:
            mouse_up.publish();
            break;

        case SDL_MOUSEMOTION:
            mouse_moved.publish(event.motion.x, event.motion.y);
            break;

        default:
            break;
        }
    }
}
