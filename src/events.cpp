#include "ion/events.hpp"
#include <SDL.h>
#include <algorithm>

namespace ion {

void event_system::subscribe(Uint32 event_type, listener_fp callback)
{
    // default the event listeners to an empty vector
    std::vector<listener_fp> listeners;

    // but get them if they exist
    auto search = _listeners_by_type.find(event_type);
    if (search != _listeners_by_type.end()) {
        listeners = search->second;
    }

    // add the current listener and update the listener map
    listeners.push_back(callback);
    _listeners_by_type[event_type] = listeners;
}

void event_system::subscribe_functor(Uint32 event_type, listener_fxn callback)
{
    // default the event listeners to an empty vector
    std::vector<listener_fxn> listeners;

    // but get them if they exist
    auto search = _listener_functors.find(event_type);
    if (search != _listener_functors.end()) {
        listeners = search->second;
    }

    // add the current listener and update the listener map
    listeners.push_back(callback);
    _listener_functors[event_type] = listeners;

}

void event_system::process_queue()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {

        // run each subscribed callback fp with the event if any exist
        auto fp_search = _listeners_by_type.find(event.type);
        if (fp_search != _listeners_by_type.end()) {
            auto listeners = fp_search->second;
            for (auto callback : listeners) { callback(event); }
        }
        // run each subscribed callback fxn with the event if any exist
        auto fxn_search = _listener_functors.find(event.type);
        if (fxn_search != _listener_functors.end()) {
            auto listeners = fxn_search->second;
            for (auto callback : listeners) {
                callback(event);
            }
        }
    }
}
}
