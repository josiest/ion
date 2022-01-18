#pragma once

// frameworks
#include <SDL.h>

// data types
#include "gui/widget.hpp"
#include "gui/frame.hpp"
#include <cstdint>
#include <memory>

class frame_template {
public:
    frame_template() = delete;
    frame_template(std::uint32_t padding);

    // interface methods
    frame * make(SDL_Rect const & bounds);
private:
    std::uint32_t _padding;
    std::vector<std::unique_ptr<frame>> _frames;
};
