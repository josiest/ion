#include "gui/widget.hpp"
#include "gui/button.hpp"
#include "gui/button_template.hpp"

#include <SDL.h>
#include <ion/ion.hpp>

// data types
#include <cstdint>
#include <string>

using uint = std::uint32_t;

button_template::button_template(ion::font & font, uint padding)
    : _font(font), _font_color{0, 0, 0, 0xff}, _padding(padding)
{
}

IWidget * button_template::make(ion::hardware_renderer & window, int x, int y,
                                uint w, std::string const & text)
{
    // create the button, padding the dimensions as necessary
    SDL_Rect const button_bounds{
        x, y, static_cast<int>(w),
              static_cast<int>(padded(TTF_FontHeight(_font)))
    };

    // render the text in the button's font and create the widget
    return _buttons.emplace_back(new button(
                button_bounds,
                _font.render_text(window, text, _font_color),
                _padding
                )).get();
}

uint button_template::padded(uint value) const
{
    return value + 2*_padding;
}
