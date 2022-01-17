#pragma once

#include <SDL.h>
#include <ion/ion.hpp>

#include "gui_element.hpp"
#include "button.hpp"
#include "div.hpp"

// data types
#include <cstdint>
#include <memory>

// data structures
#include <vector>

class button_template : public ion::isotope, public IGuiElementFactory {
public:
    button_template() = delete;
    button_template(std::uint32_t h);

    // interface methods
    IGuiElement * make(int x, int y, std::uint32_t w);
private:
    std::uint32_t _button_height;
    std::vector<std::unique_ptr<button>> _buttons;
};
