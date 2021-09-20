#pragma once
#include <cstdint>

namespace component {

struct position {
    float x, y;
};

struct velocity {
    float x, y;
};

struct size {
    float value;
};

struct bbox {
    float x, y, size;
};

struct color {
    uint8_t r, g, b;
};

struct dynamic_mover {
    float acceleration;
    float friction;
    float minspeed;
    float maxspeed;
};

struct munchable {};
}
