#pragma once

#include <SDL.h>
#include <ion/ion.hpp>

#include "gui/widget.hpp"
#include "gui/button.hpp"

// data types
#include <cstdint>
#include <memory>

// data structures
#include <vector>

class button_template : public ion::isotope, public IWidgetFactory {
public:
    button_template() = delete;
    button_template(std::uint32_t h);

    // interface methods
    IWidget * make(int x, int y, std::uint32_t w);
private:
    std::uint32_t _button_height;
    std::vector<std::unique_ptr<button>> _buttons;
};
