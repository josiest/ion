#include "ion.hpp"
#include "SDL2/SDL.h"
#include <stdexcept>

struct Renderer {
    SDL_Renderer * sdl_renderer;
};
Renderer & global_renderer()
{
    static Renderer r{nullptr};
    return r;
}
void renderer(SDL_Renderer * sdl_renderer)
{
    auto & r = global_renderer();
    r.sdl_renderer = sdl_renderer;
}
SDL_Renderer * renderer()
{
    return global_renderer().sdl_renderer;
}

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
