#include "gui/button.hpp"

// frameworks
#include <ion/ion.hpp>
#include <SDL.h>

// data types
#include <cstdint>

// aliases
using uint = std::uint32_t;

button::button(SDL_Rect bounds)
    : _bounds(bounds)
{
}

SDL_Rect button::bounds() const { return _bounds; }
void button::render(ion::hardware_renderer & window)
{
    SDL_SetRenderDrawColor(window, 0, 0, 0, 0xff);
    SDL_RenderDrawRect(window, &_bounds);
}
