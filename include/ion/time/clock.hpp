#pragma once
#include <cstdint>

namespace ion {

class clock {
public:
    clock();
    float tick();
private:
    uint32_t _prev_time;
};
}
