#include <SDL2/SDL.h>
#include <stdexcept>

namespace ion {

void init_video() {
    // initialize video if it hasn't already been done
    if (SDL_WasInit(SDL_INIT_VIDEO)) {
        return;
    }
    // throw runtime error if video can't initialize
    if (SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        throw std::runtime_error{SDL_GetError()};
    }
}
}
