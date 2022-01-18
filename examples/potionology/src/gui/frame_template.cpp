
// data types
#include "gui/widget.hpp"
#include "gui/frame_template.hpp"
#include <cstdint>

// aliases
using uint = std::uint32_t;

frame_template::frame_template(uint padding)
    : _padding(padding)
{
}

frame * frame_template::make(SDL_Rect const & bounds)
{
    int const i = _frames.size();
    return _frames.emplace_back(new frame(bounds, _padding)).get();
}
