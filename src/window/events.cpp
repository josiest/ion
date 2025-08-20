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
        default:
            break;
        }
    }
}
