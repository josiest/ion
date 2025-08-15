#include "ion/input/axis.hpp"
#include <SDL2/SDL.h>

#include <cstdint>

namespace ion::input {

keyboard_axis::keyboard_axis(SDL_Keycode right, SDL_Keycode left, SDL_Keycode up, SDL_Keycode down)
    : _right(right), _left(left), _up(up), _down(down)
{
}

float keyboard_axis::x() const noexcept
{
    const std::uint8_t* is_pressed = SDL_GetKeyboardState(nullptr);
    const SDL_Scancode right_key = SDL_GetScancodeFromKey(_right);
    const SDL_Scancode left_key = SDL_GetScancodeFromKey(_left);
    return static_cast<float>(is_pressed[right_key]) - static_cast<float>(is_pressed[left_key]);
}

float keyboard_axis::y() const noexcept
{
    const std::uint8_t* is_pressed = SDL_GetKeyboardState(nullptr);
    const SDL_Scancode up_key = SDL_GetScancodeFromKey(_up);
    const SDL_Scancode down_key = SDL_GetScancodeFromKey(_down);
    return static_cast<float>(is_pressed[up_key]) - static_cast<float>(is_pressed[down_key]);
}

}
