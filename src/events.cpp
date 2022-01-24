#include "ion/events.hpp"

// frameworks
#include <SDL.h>

// data types
#include <cstdint> // std::uint32_t

// data structures
#include <vector>

// algorithms
#include <algorithm>

// aliases
using uint = std::uint32_t;

namespace ion {

void event_system::subscribe(uint event_type, listener_fp callback)
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
    _listeners_by_type.insert_or_assign(event_type, listeners);
}

void event_system::subscribe_functor(uint event_type, listener_fn callback)
{
    // default the event listeners to an empty vector
    std::vector<listener_fn> listeners;

    // but get them if they exist
    auto search = _listener_functors.find(event_type);
    if (search != _listener_functors.end()) {
        listeners = search->second;
    }

    // add the current listener and update the listener map
    listeners.push_back(callback);
    _listener_functors.insert_or_assign(event_type, listeners);

}

void event_system::process_queue()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {

        // TODO: iterate callbacks in *true* order subscribed (interleaved)

        // run each subscribed callback fp with the event if any exist
        auto fp_search = _listeners_by_type.find(event.type);
        if (fp_search != _listeners_by_type.end()) {
            auto listeners = fp_search->second;
            for (auto callback : listeners) { callback(event); }
        }
        // run each subscribed callback fn with the event if any exist
        auto fn_search = _listener_functors.find(event.type);
        if (fn_search != _listener_functors.end()) {
            auto listeners = fn_search->second;
            for (auto callback : listeners) {
                callback(event);
            }
        }
    }
}
}
