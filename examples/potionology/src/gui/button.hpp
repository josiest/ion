#pragma once

// frameworks
#include <ion/ion.hpp>
#include <SDL.h>

// data types
#include <cstdint>
#include "gui/widget.hpp"

// forward declare button template in order to make it a friend
class button_template;

class button : public IWidget {
public:
    button() = delete;

    // interface methods
    SDL_Rect bounds() const;
    void render(ion::hardware_renderer & window);
private:
    friend class button_template;
    SDL_Rect _bounds;
    int _padding;
    ion::texture _text;

    button(SDL_Rect bounds, ion::texture && text, std::uint32_t padding);
};
