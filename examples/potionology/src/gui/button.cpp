#include "gui/button.hpp"

// frameworks
#include <ion/ion.hpp>
#include <SDL.h>

// data types
#include <cstdint>

// aliases
using uint = std::uint32_t;

button::button(SDL_Rect bounds, ion::texture && text, uint padding)
    : _bounds(bounds), _text(std::move(text)),
      _padding(static_cast<int>(padding))
{
}

SDL_Rect button::bounds() const { return _bounds; }
void button::render(ion::hardware_renderer & window)
{
    // outline the button with black
    SDL_SetRenderDrawColor(window, 0, 0, 0, 0xff);
    SDL_RenderDrawRect(window, &_bounds);

    // get the rendered text dimensions for calculating rendering rect
    int w, h;
    SDL_QueryTexture(_text, nullptr, nullptr, &w, &h);

    // define the rect to render the text to - cutoff text that's too long
    SDL_Rect const text_bounds{
        _bounds.x + _padding, _bounds.y + _padding,
        std::min(w, _bounds.w), h
    };

    SDL_RenderCopy(window, _text, nullptr, &text_bounds);
}
