#include "ion/time/clock.hpp"
#include <SDL2/SDL_timer.h>
#include <cstdint>

namespace ion {

clock::clock() : _prev_time{SDL_GetTicks()} {}
float clock::tick()
{
    // get the current time
    uint32_t const current_time = SDL_GetTicks();

    // calculate the number of seconds since the last call
    float const dt = static_cast<float>(current_time-_prev_time)/1000.f;

    // update the clock
    _prev_time = current_time;
    return dt;
}
}
