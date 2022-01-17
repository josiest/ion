#pragma once

// frameworks
#include <ion/ion.hpp>
#include <SDL.h>

// data types
#include <cstdint>
#include "gui_element.hpp"

// forward declare button template in order to make it a friend
class button_template;

class button : public IGuiElement {
public:
    button() = delete;

    // interface methods
    SDL_Rect bounds() const;
    void render(ion::hardware_renderer & window);
private:
    friend class button_template;
    SDL_Rect _bounds;

    button(SDL_Rect bounds);
};
