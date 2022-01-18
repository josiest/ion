
// data types
#include "gui_element.hpp"
#include "div_template.hpp"
#include <cstdint>

// aliases
using uint = std::uint32_t;

div_template::div_template(uint padding)
    : _padding(padding)
{
}

divider * div_template::make(SDL_Rect const & bounds)
{
    int const i = _divs.size();
    return _divs.emplace_back(new divider(bounds, _padding)).get();
}
