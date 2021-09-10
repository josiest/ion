#pragma once

#include <SDL2/SDL.h>
#include <unordered_map>
#include <vector>
#include <functional>

namespace ion {

typedef void (*ListenerFP)(SDL_Event const &);
using ListenerFXN = std::function<void(SDL_Event const &)>;

class EventHandler {
public:
    inline EventHandler() {}

    /**
     * Subscribe a function pointer listener
     *
     * \param event_type the SDL event type to listen for
     * \param listener the function to call back to when event_type is triggered
     *
     * \note This should be the preferred method for subscribing functions
     *       because it has the least overhead
     */
    void subscribe(Uint32 event_type, ListenerFP listener);

    /**
     * Subscribe an arbitrary function listener
     *
     * \param event_type the SDL event type to listen for
     * \param listener the function to call back to when event_type is triggered
     *
     * \note Be cautious when using this method because std::function has a lot
     *       of overhead and it may unnecesarily slow down your game
     */
    void subscribe_functor(Uint32 event_type, ListenerFXN listener);

    /**
     * Poll all SDL events in the queue, calling any listeners along the way.
     */
    void process_queue();
private:
    std::unordered_map<Uint32, std::vector<ListenerFP>> _listeners_by_type;
    std::unordered_map<Uint32, std::vector<ListenerFXN>> _listener_functors;
};
}
