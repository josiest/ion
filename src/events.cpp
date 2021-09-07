#include "events.hpp"
#include <SDL2/SDL.h>
#include <algorithm>

namespace ion {

void EventHandler::subscribe(Uint32 event_type, Listener listener)
{
    // default the event listeners to an empty vector
    std::vector<Listener> listeners;

    // but get them if they exist
    auto search = _listeners_by_type.find(event_type);
    if (search != _listeners_by_type.end()) {
        listeners = search->second;
    }

    // add the current listener and update the listener map
    listeners.push_back(listener);
    _listeners_by_type[event_type] = listeners;
}

void EventHandler::process_queue()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {

        auto search = _listeners_by_type.find(event.type);

        // if no listeners are subscribed, skip this event
        if (search == _listeners_by_type.end()) {
            continue;
        }

        // otherwise run each subscribed callback fxn with the event
        auto listeners = search->second;
        std::for_each(listeners.begin(), listeners.end(),
                      [&event](auto callback) { callback(event); });
    }
}
}
