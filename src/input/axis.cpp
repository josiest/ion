#include "ion/input/axis.hpp"
#include <cstdint>
#include <SDL3/SDL_keyboard.h>

namespace ion::input {

keyboard_axis::keyboard_axis(SDL_Keycode right, SDL_Keycode left, SDL_Keycode up, SDL_Keycode down)
    : _right(right), _left(left), _up(up), _down(down)
{
}

float keyboard_axis::x() const
{
    const bool * is_pressed = SDL_GetKeyboardState(nullptr);
    const SDL_Scancode right_key = SDL_GetScancodeFromKey(_right, nullptr);
    const SDL_Scancode left_key = SDL_GetScancodeFromKey(_left, nullptr);
    return static_cast<float>(is_pressed[right_key]) - static_cast<float>(is_pressed[left_key]);
}

float keyboard_axis::y() const
{
    const bool * is_pressed = SDL_GetKeyboardState(nullptr);
    const SDL_Scancode up_key = SDL_GetScancodeFromKey(_up, nullptr);
    const SDL_Scancode down_key = SDL_GetScancodeFromKey(_down, nullptr);
    return static_cast<float>(is_pressed[up_key]) - static_cast<float>(is_pressed[down_key]);
}

}
