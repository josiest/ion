#pragma once

// frameworks
#include <SDL.h>

// data types
#include "gui_element.hpp"
#include "div.hpp"
#include <cstdint>
#include <memory>

class div_template {
public:
    div_template() = delete;
    div_template(std::uint32_t padding);

    // interface methods
    divider * make(SDL_Rect const & bounds);
private:
    std::uint32_t _padding;
    std::vector<std::unique_ptr<divider>> _divs;
};
