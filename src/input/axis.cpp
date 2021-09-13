#include "ion/input/axis.hpp"
#include "ion/events.hpp"
#include <SDL2/SDL.h>

#include <unordered_set>

namespace ion::input {

namespace _g {
std::unordered_set<SDL_Keycode> KEYS_PRESSED;
}

void set_key(SDL_Event const & event)
{
    _g::KEYS_PRESSED.insert(event.key.keysym.sym);
}
void release_key(SDL_Event const & event)
{
    _g::KEYS_PRESSED.erase(event.key.keysym.sym);
}

KeyboardAxis2D::KeyboardAxis2D(event_system & events,
                               SDL_Keycode right, SDL_Keycode left,
                               SDL_Keycode up, SDL_Keycode down)

    : _right(right), _left(left), _up(up), _down(down)
{
    events.subscribe(SDL_KEYDOWN, &set_key);
    events.subscribe(SDL_KEYUP, &release_key);
}

float KeyboardAxis2D::x() const
{
    bool right_pressed = _g::KEYS_PRESSED.find(_right) != _g::KEYS_PRESSED.end();
    bool left_pressed = _g::KEYS_PRESSED.find(_left) != _g::KEYS_PRESSED.end();
    return static_cast<float>(right_pressed) - static_cast<float>(left_pressed);
}

float KeyboardAxis2D::y() const
{
    bool up_pressed = _g::KEYS_PRESSED.find(_up) != _g::KEYS_PRESSED.end();
    bool down_pressed = _g::KEYS_PRESSED.find(_down) != _g::KEYS_PRESSED.end();
    return static_cast<float>(up_pressed) - static_cast<float>(down_pressed);
}

}
