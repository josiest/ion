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
#include <ion/ion.hpp>
#include <SDL2/SDL.h>
#include <cmath>

void render(ion::renderable_window auto & window);

int main()
{
    // initialize sdl - initialize this before other sdl resources
    ion::sdl_context sdl;

    // create the sdl event-handler: quit when sdl's quit event is triggered
    ion::event_system events;
    events.subscribe(SDL_QUIT, &ion::input::quit_on_event);

    // create a window, specifying the title and dimensions
    auto window = ion::render_window("A simple window", 800, 600);

    // render once at the beginning of the program
    render(window);

    // busy loop until the user quits
    while (!ion::input::has_quit()) {
        events.process_queue();
    }
}

// draw a fibonacci-like pattern
void render(ion::renderable_window auto & window)
{
    // the initial color
    int r0 = 48, g0 = 118, b0 = 217;
    // the final color
    int r1 = 219, g1 = 0, b1 = 66;

    // clear the screen
    auto renderer = window.sdl_renderer();
    SDL_SetRenderDrawColor(renderer, r1, g1, b1, 0xff);
    SDL_RenderClear(renderer);

    // the dimensions of the rect to draw
    SDL_Rect rect{0, 0, 0, 0};
    SDL_GetWindowSize(window.sdl_window(), &rect.w, &rect.h);
    rect.w /= 2;

    // draw the fibonacci-like patern
    int n = 8;
    auto intlerp = [](int a, int b, float t) {
        return static_cast<int>(std::lerp(a, b, t));
    };
    for (int k = 0; k < n; k++) {

        // calculate the intermediate color
        float const t = static_cast<float>(k)/n;
        int const r = intlerp(r0, r1, t);
        int const g = intlerp(g0, g1, t);
        int const b = intlerp(b0, b1, t);

        // draw the rect
        SDL_SetRenderDrawColor(renderer, r, g, b, 0xff);
        SDL_RenderFillRect(renderer, &rect);

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
    SDL_RenderPresent(renderer);
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
