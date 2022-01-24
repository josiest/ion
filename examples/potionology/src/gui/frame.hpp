#pragma once

// frameworks
#include "gold/gold.hpp"

class frame {
public:
    au::frame gold_frame;

    frame() = delete;
private:
    bool _active;
};
