#pragma once

#include <SDL2/SDL.h>
#include <unordered_map>
#include <vector>

namespace ion {

typedef void (*Listener)(SDL_Event const &);

class EventHandler {
public:
    inline EventHandler() : _listeners_by_type() {}

    void subscribe(Uint32 event_type, Listener listener);
    void process_queue();
private:
    std::unordered_map<Uint32, std::vector<Listener>> _listeners_by_type;
};
}
