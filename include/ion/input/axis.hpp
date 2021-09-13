#pragma once
#include <SDL2/SDL.h>
#include "ion/events.hpp"

namespace ion::input {

class axis2d {
public:
    // get the x-value of the axis
    virtual float x() const = 0;

    // get the y-value of the axis
    virtual float y() const = 0;
};

void set_key(SDL_Keycode key);
void release_key(SDL_Keycode key);

class keyboard_axis : public axis2d {
public:
    keyboard_axis(event_system & events, SDL_Keycode right, SDL_Keycode left,
                                         SDL_Keycode up, SDL_Keycode down);
    float x() const;
    float y() const;
private:
    SDL_Keycode _right, _left, _up, _down;
};
}
