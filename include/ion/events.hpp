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

    void subscribe(Uint32 event_type, ListenerFP listener);
    void subscribe_functor(Uint32 event_type, ListenerFXN listener);
    void process_queue();
private:
    std::unordered_map<Uint32, std::vector<ListenerFP>> _listeners_by_type;
    std::unordered_map<Uint32, std::vector<ListenerFXN>> _listener_functors;
};
}
