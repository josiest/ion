#pragma once
#include <SDL2/SDL.h>

struct Position {
    float x, y;
};

struct Velocity {
    float x, y;
};

struct Size {
    int value;
};

struct Color {
    Uint8 r, g, b, a;
};

struct DynamicMover {
    float acceleration;
    float friction;
    float minspeed;
    float maxspeed;
};

struct Player {};
