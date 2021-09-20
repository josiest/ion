#pragma once
#include <cstdint>

struct Position {
    float x, y;
};

struct Velocity {
    float x, y;
};

struct Size {
    float value;
};

struct Color {
    uint8_t r, g, b, a;
};

struct DynamicMover {
    float acceleration;
    float friction;
    float minspeed;
    float maxspeed;
};

struct Player {};
struct Munchable {};
