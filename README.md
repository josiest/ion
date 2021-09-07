# ion

`ion` is a small-scoped library meant to reduce boiler-plate SDL code.
It serves two primary functions

1. it acts as RAII resource handler for SDL components
2. it provides a higher level event-handling interface than pure SDL

`ion` is still in the very early stages of development. Since this is currently
a personal project, more features will be added as I need them.

## Installation

The goal is to eventually add ion to different c++ package managers, but I'm
just a mere student and I don't have the time to do that right now. For now
you'll need to download the source code and make it with cmake.

I recommend adding this repository as a submodule for now:

```console
$ cd <project_directory>
$ git submodule add https://github.com/josiest/ion.git external/ion
```

Then from there, you can use cmake to build the package

```console
$ mkdir external/ion/build && cd external/ion/build
$ cmake ..
$ cmake --build .
```

Finally add the following to your `CMakeLists.txt`

```cmake
set(ion_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/ion/build)
find_package(ion REQUIRED)
...
target_link_libraries(<project-name> PRIVATE ion)
```

## Examples

Here's a simple example of how you might use `ion`. This example renders
a fibonacci-like gradient pattern to the screen.

```cpp
#include "ion.hpp"
#include "SDL2/SDL.h"

// draw a fibonacci-like pattern
void render(ion::Window & window)
{
    // the initial color
    int r0 = 48, g0 = 118, b0 = 217;
    // the final color
    int r1 = 219, g1 = 0, b1 = 66;

    // clear the screen
    SDL_SetRenderDrawColor(window.sdl_renderer(), r1, g1, b1, 0xff);
    SDL_RenderClear(window.sdl_renderer());

    // the dimensions of the rect to draw
    SDL_Rect rect{0, 0, 0, 0};
    SDL_GetWindowSize(window.sdl_window(), &rect.w, &rect.h);
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
        SDL_SetRenderDrawColor(window.sdl_renderer(), r, g, b, 0xff);
        SDL_RenderFillRect(window.sdl_renderer(), &rect);

        // split in half horizontally when k is even
        if (k % 2 == 0) {
            rect.x += rect.w;
            rect.h /= 2;
        }
        // split in half vertically when k is odd
        else {
            rect.y += rect.h;
            rect.w /= 2;
        }
    }
    SDL_RenderPresent(window.sdl_renderer());
}

// since this framework relies on function pointers in order to handle events,
// we can't pass capturing lambdas to the event handlers!
// thus (at least to my knowledge), we're limited to using a global resource
bool HAS_QUIT = false;
bool has_quit() { return HAS_QUIT; }
void quit(SDL_Event const &) { HAS_QUIT = true; }
// NOTE: in general, it might be a better idea to create a resources class with
// an API for managing its resources. But since this example only needs to know
// when the user quit, we'll just keep things as simple as possible

int main()
{
    // create the sdl event handler
    ion::EventHandler handler;
    hanler.subscribe(SDL_QUIT, &quit);

    // specify the app's title and dimensions
    auto window = ion::unique_basic_window("A simple window", 800, 600);

    // run the program
    while (!has_quit()) {
        handler.process_queue();
    }
}
```

To run this example from the ion project directory run the following code

```console
$ mkdir examples/simple/build && cd examples/simple/build
$ cmake ..
$ cmake --build .
$ ./simple
```

![simple example window](images/simple-example.png)
