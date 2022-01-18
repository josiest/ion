#pragma once

#include <SDL.h>
#include <ion/ion.hpp>

#include "gui/widget.hpp"
#include "gui/button.hpp"

// data types
#include <cstdint>
#include <memory>
#include <string>

// data structures
#include <vector>

class button_template : public ion::isotope, public IWidgetFactory {
public:
    button_template() = delete;
    button_template(ion::font & font, std::uint32_t padding);

    // interface methods
    IWidget * make(ion::hardware_renderer & window, int x, int y,
                   std::uint32_t w, std::string const & text);
private:
    ion::font & _font;
    SDL_Color _font_color;

    std::uint32_t _padding;
    std::vector<std::unique_ptr<button>> _buttons;

    std::uint32_t padded(std::uint32_t value) const;
};
