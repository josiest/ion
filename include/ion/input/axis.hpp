#pragma once
#ifdef USE_SDL2
#include <SDL2/SDL.h>
#else
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_mouse.h>
#endif

namespace ion::input {

class axis2d {
public:
    virtual ~axis2d() = default;

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
     * \param right to bind to the positive x-axis
     * \param left to bind to the negative x-axis
     * \param up to bind to the positive y-axis
     * \param down to bind to the negative y-axis
     */
    keyboard_axis(SDL_Keycode right, SDL_Keycode left, SDL_Keycode up, SDL_Keycode down);

    float x() const override;
    float y() const override;
private:
    SDL_Keycode _right, _left, _up, _down;
};

/** Get the position of the mouse */
inline SDL_FPoint mouse_position()
{
    SDL_FPoint mouse{-1, -1};
    SDL_GetMouseState(&mouse.x, &mouse.y);
    return mouse;
}
}
