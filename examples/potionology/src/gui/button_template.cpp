#include "gui/widget.hpp"
#include "gui/button.hpp"
#include "gui/button_template.hpp"

#include <SDL.h>
#include <ion/ion.hpp>

// data types
#include <cstdint>

using uint = std::uint32_t;

button_template::button_template(uint h)
    : _button_height(h)
{
}

IWidget * button_template::make(int x, int y, uint w)
{
    return _buttons.emplace_back(new button(SDL_Rect{
        x, y, static_cast<int>(w), static_cast<int>(_button_height)
    })).get();
}
