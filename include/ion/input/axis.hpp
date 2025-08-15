#pragma once
#include <SDL2/SDL.h>

namespace ion::input {

class axis2d {
public:
    // get the x-value of the axis
    virtual float x() const = 0;

    // get the y-value of the axis
    virtual float y() const = 0;
};

/**
 * A 2d axis that sets the value when keys are pressed
 */
class keyboard_axis : public axis2d {
public:
    keyboard_axis() = delete;
    /**
     * Subscribe a custom keyboard axis to an event system
     *
     * \param events the system to subscribe to
     *
     * \param right to bind to the positive x-axis
     * \param left to bind to the negative x-axis
     * \param up to bind to the positive y-axis
     * \param down to bind to the negative y-axis
     */
    keyboard_axis(SDL_Keycode right, SDL_Keycode left, SDL_Keycode up, SDL_Keycode down);

    float x() const noexcept;
    float y() const noexcept;
private:
    SDL_Keycode _right, _left, _up, _down;
};

/** Get the position of the mouse */
inline SDL_Point mouse_position()
{
    SDL_Point mouse{-1, -1};
    SDL_GetMouseState(&mouse.x, &mouse.y);
    return mouse;
}
}
