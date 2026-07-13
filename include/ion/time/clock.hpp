#pragma once

#include <cstdint>
#include <concepts>
#include <SDL3/SDL_timer.h>

namespace ion {

/** A simple time interface */
class clock {
public:
    /** Initialize a clock */
    clock()
        : start_time{SDL_GetTicks()},  prev_time{start_time}
    {}

    /**
     * The time in seconds since the last time tick was called. If tick hasn't
     * been called yet, returns the time since the clock was initialized.
     */
    template<std::floating_point real_t = float>
    real_t tick();

    /** The time in seconds since the clock was initialized */
    template<std::floating_point real_t = float>
    real_t time() const;
private:
    std::uint64_t start_time;
    std::uint64_t prev_time;
};

template<std::floating_point real_t>
real_t clock::tick()
{
    // get the current time
    uint32_t const current_time = SDL_GetTicks();

    // calculate the number of seconds since the last call
    real_t const dt = static_cast<real_t>(current_time-prev_time)/1000.;

    // update the clock
    prev_time = current_time;
    return dt;
}


template<std::floating_point real_t>
real_t clock::time() const
{
    return static_cast<real_t>(SDL_GetTicks()-start_time)/1000.;
}

}
