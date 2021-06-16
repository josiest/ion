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
