#include "gui/widget.hpp"
#include "gui/frame.hpp"
#include "gui/frame_template.hpp"

#include <SDL.h>
#include <cstdint>

#include <utility>

using uint = std::uint32_t;

frame::frame(SDL_Rect const & bounds, uint padding)
    : _bounds(bounds), _padding(padding)
{
}

void frame::produce_element_from(IWidgetFactory * factory)
{
    SDL_Point const next = next_point();
    _elements.push_back(factory->make(next.x, next.y, padded_width()));
}

std::pair<frame *, frame *>
frame::vsplit(frame_template & factory, float left_ratio)
{
    // calculate the widths to use for each new frame

    // there'll be an extra layer of padding between the left and right frames
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

    // create the bounding rects and use the factory to create the frame objects
    SDL_Rect const left_bounds{
        left_point.x, left_point.y, left_width, height
    };
    SDL_Rect const right_bounds{
        right_point.x, right_point.y, right_width, height
    };

    frame * left_frame = factory.make(left_bounds);
    frame * right_frame = factory.make(right_bounds);

    // add the new objects to the elements and return their pointers
    _elements.push_back(left_frame);
    _elements.push_back(right_frame);
    return std::make_pair(left_frame, right_frame);
}

SDL_Rect frame::bounds() const { return _bounds; }
void frame::render(ion::hardware_renderer & window)
{
    for (IWidget * element : _elements) {
        element->render(window);
    }
}

SDL_Point frame::next_point() const
{
    // always start from a static x point - the frame x + padding
    int const x = _bounds.x + _padding;

    // initialize the next point at the first appropriate point
    if (_elements.empty()) {
        return {x, _bounds.y + _padding};
    }
    // if the frame has elements, calculate the next position using the previous
    SDL_Rect const last_bounds = _elements.back()->bounds();
    return {x, last_bounds.y + last_bounds.h + _padding};
}
