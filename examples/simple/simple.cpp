#include "ion.hpp"
#include "SDL2/SDL.h"
#include <iostream>

// our render function will need acces to the app's renderer
// there are more sophisticated ways of doing this, but for the sake of
// simplicity we'll just use a global variable
SDL_Window * WINDOW;
SDL_Renderer * RENDERER;

// a fibonacci-like pattern
void render()
{
    // the initial color
    int r0 = 48, g0 = 118, b0 = 217;
    // the final color
    int r1 = 219, g1 = 0, b1 = 66;

    // clear
    SDL_SetRenderDrawColor(RENDERER, r1, g1, b1, 0xff);
    SDL_RenderClear(RENDERER);

    // the dimensions of the rect to draw
    SDL_Rect rect{0, 0, 0, 0};
    SDL_GetWindowSize(WINDOW, &rect.w, &rect.h);
    rect.w /= 2;

    // draw the fibonacci-like patern
    int n = 8;
    for (int k = 0; k < n; k++) {

        // calculate the intermediate color
        float t = (float)k/n;
        int r = (int)((1-t)*r0 + t*r1);
        int g = (int)((1-t)*g0 + t*g1);
        int b = (int)((1-t)*b0 + t*b1);

        // draw the rect
        SDL_SetRenderDrawColor(RENDERER, r, g, b, 0xff);
        SDL_RenderFillRect(RENDERER, &rect);

        // split in half horizontally when k is even
        if (k % 2 == 0) {
            rect.x += rect.w;
            rect.h /= 2;
        }
        // split in half vertically when k is even
        else {
            rect.y += rect.h;
            rect.w /= 2;
        }
    }
    SDL_RenderPresent(RENDERER);
}

int main(int argc, char * argv[])
{
    // specify the app's title and dimensions
    ion::app simple{"A simple window", 800, 600};

    // load the renderer and render function
    WINDOW = simple.window();
    RENDERER = simple.renderer();
    simple.connect_update_listener(&render); 

    // run the app
    simple.run();
    return 0;
}
