# ion

`ion` is a small-scoped library meant to reduce boiler-plate SDL code.
It serves two primary functions

1. it acts as RAII resource handler for SDL components
2. it provides a higher level event-handling interface than pure SDL

## Installation

The goal is to eventually add ion to different c++ package managers, but I'm
just a mere student and I don't have the time to do that right now. For now
you'll just need to download the source code and use your favorite compiling
tool to link the library correctly.

## Examples

Here's a simple example of how you might use `ion`. This example renders
the screen as white.

```cpp
#include "ion.hpp"
#include "SDL2/SDL.h"

// define a wrapper class to set a global renderer
struct Renderer {
    SDL_Renderer * sdl_renderer;
};
// initialize and retreive the global renderer wrapper
Renderer & global_renderer()
{
    static Renderer r{nullptr};
    return r;
}
// set the global SDL_Renderer
void renderer(SDL_Renderer * sdl_renderer)
{
    auto & r = global_renderer();
    r.sdl_renderer = sdl_renderer;
}
// retreive the global SDL_Renderer
SDL_Renderer * renderer()
{
    return global_renderer().sdl_renderer;
}

// draw a white screen
void render()
{
    SDL_SetRenderDrawColor(renderer(), 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(renderer());
    SDL_RenderPresent(renderer());
}

int main(int argc, char * argv[])
{
    ion::app simple{"A simple window", 800, 600};
    renderer(simple.renderer());
    simple.connect_update_listener(&render);
    simple.run();
    return 0;
}
```
