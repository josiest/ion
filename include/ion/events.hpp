#pragma once

#include <SDL_events.h>
#include <unordered_map>
#include <vector>
#include <functional>

namespace ion {

typedef void (*listener_fp)(SDL_Event const &);
using listener_fxn = std::function<void(SDL_Event const &)>;

class event_system {
public:
    /**
     * Subscribe a function pointer listener
     *
     * \param event_type the SDL event type to listen for
     * \param callback the function to call back to when event_type is triggered
     *
     * \note This should be the preferred method for subscribing functions
     *       because it has the least overhead
     */
    void subscribe(uint32_t event_type, listener_fp callback);

    /**
     * Subscribe an arbitrary function listener
     *
     * \param event_type the SDL event type to listen for
     * \param callback the function to call back to when event_type is triggered
     *
     * \note Be cautious when using this method because std::function has a lot
     *       of overhead and it may unnecesarily slow down your game
     */
    void subscribe_functor(uint32_t event_type, listener_fxn callback);

    /**
     * Poll all SDL events in the queue, calling any listeners along the way.
     */
    void process_queue();
private:
    std::unordered_map<uint32_t, std::vector<listener_fp>> _listeners_by_type;
    std::unordered_map<uint32_t, std::vector<listener_fxn>> _listener_functors;
};
}
