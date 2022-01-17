#include "div.hpp"
#include "gui_element.hpp"

#include <SDL.h>
#include <cstdint>

using uint = std::uint32_t;

divider::divider(SDL_Rect const & bounds, uint padding)
    : _bounds(bounds), _padding(padding)
{
}

void divider::produce_element_from(IGuiElementFactory * factory)
{
    // initialize the element's position to the first acceptable place
    int x = _bounds.x + _padding;
    int y = _bounds.y + _padding;

    // if the divider has elements, calculate the next position using the previous
    if (not _elements.empty()) {
        IGuiElement * last_element = _elements.back();
        SDL_Rect const last_bounds = last_element->bounds();

        // calculate: same x, but add last element's height + padding
        // note - this assumes positions y values are at top of element
        y = last_bounds.y + last_bounds.h + _padding;
    }
    _elements.push_back(factory->make(x, y, padded_width()));
}

SDL_Rect divider::bounds() const { return _bounds; }
void divider::render(ion::hardware_renderer & window)
{
    for (IGuiElement * element : _elements) {
        element->render(window);
    }
}
