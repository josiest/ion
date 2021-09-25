#pragma once

#include <cstdint>
#include <concepts>

namespace ion {

/** A simple time interface */
template<std::floating_point real_t = float>
class clock {
public:
    /** Initialize a clock */
    clock() noexcept;

    /**
     * The time in seconds since the last time tick was called. If tick hasn't
     * been called yet, returns the time since the clock was initialized.
     */
    real_t tick() noexcept;

    /** The time in seconds since the clock was initialized */
    real_t time() const noexcept;
private:
    uint32_t _start_time;
    uint32_t _prev_time;
};

clock::clock() noexcept
    : _start_time{SDL_GetTicks()}  _prev_time{_start_time}
{}

real_t clock::tick() noexcept
{
    // get the current time
    uint32_t const current_time = SDL_GetTicks();

    // calculate the number of seconds since the last call
    real_t const dt = static_cast<real_t>(current_time-_prev_time)/1000.f;

    // update the clock
    _prev_time = current_time;
    return dt;
}
real_t clock::time() const noexcept
{
    return static_cast<real_t>(SDL_GetTicks()-_start_time)/1000.f;
}

}
