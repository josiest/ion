#include "ion/input/quit.hpp"
#include <SDL.h>

namespace ion::input {

// namespace for global variables
namespace _g {
    bool has_quit = false;
}

bool has_quit() { return _g::has_quit; }
void quit() { _g::has_quit = true; }
void quit_on_event(SDL_Event const &) { quit(); }
}
