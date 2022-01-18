#include "div.hpp"
#include "gui_element.hpp"
#include "div_template.hpp"

#include <SDL.h>
#include <cstdint>

#include <utility>

using uint = std::uint32_t;

divider::divider(SDL_Rect const & bounds, uint padding)
    : _bounds(bounds), _padding(padding)
{
}

void divider::produce_element_from(IGuiElementFactory * factory)
{
    SDL_Point const next = next_point();
    _elements.push_back(factory->make(next.x, next.y, padded_width()));
}

std::pair<divider *, divider *>
divider::vsplit(div_template & factory, float left_ratio)
{
    // calculate the widths to use for each new div

    // there'll be an extra layer of padding between the left and right divs
    // so we'll need to take it off the normal padded width
    int const width = padded_width() - _padding;

    // left is a specified ratio of the padded width, right is the rest of it
    int const left_width = static_cast<int>(width * left_ratio);
    int const right_width = padded_width() - left_width;

    // get the next point and calculate the point right to it
    SDL_Point const left_point = next_point();
    SDL_Point const right_point{
        left_point.x + left_width + _padding, left_point.y
    };

    // calculate the height to use for both
    int const height = _bounds.h - _padding - left_point.y;

    // create the bounding rects and use the factory to create the div objects
    SDL_Rect const left_bounds{
        left_point.x, left_point.y, left_width, height
    };
    SDL_Rect const right_bounds{
        right_point.x, right_point.y, right_width, height
    };

    divider * left_div = factory.make(left_bounds);
    divider * right_div = factory.make(right_bounds);

    // add the new objects to the elements and return their pointers
    _elements.push_back(left_div);
    _elements.push_back(right_div);
    return std::make_pair(left_div, right_div);
}

SDL_Rect divider::bounds() const { return _bounds; }
void divider::render(ion::hardware_renderer & window)
{
    for (IGuiElement * element : _elements) {
        element->render(window);
    }
}

SDL_Point divider::next_point() const
{
    // always start from a static x point - the div x + padding
    int const x = _bounds.x + _padding;

    // initialize the next point at the first appropriate point
    if (_elements.empty()) {
        return {x, _bounds.y + _padding};
    }
    // if the divider has elements, calculate the next position using the previous
    SDL_Rect const last_bounds = _elements.back()->bounds();
    return {x, last_bounds.y + last_bounds.h + _padding};
}
