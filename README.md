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
you'll need to download the source code and build it with cmake, then install
it in a place that other CMake projects can find it

```console
$ git clone https://github.com/josiest/ion.git <path/to/ion> && cd <path/to/ion>
$ mkdir build && cd build
$ cmake ..
$ cmake --build .
```

Then if you're on linux, you should run

```console
$ sudo cmake --install . --prefix "/usr/local"
```

Finally, add the following to your `CMakeLists.txt`

```cmake
find_package(ion REQUIRED)
...
target_link_libraries(<project-name> PRIVATE ion::ion)
```

If you're not on linux, you might consider installing it under an "external"
folder in your project.

```console
$ cmake --install . --prefix "<path/to/project>/external"
```

Then you'd need to add the external folder to your cmake prefix path, as well as
to your include directories.

```cmake
list(APPEND CMAKE_PREFIX_PATH <path/to/project>/external)
include_directories(external)
find_package(ion REQUIRED)
...
target_link_libraries(<project-name> PRIVATE ion::ion)
```


## Examples

Here's a simple example of how you might use `ion`. This example renders
a fibonacci-like gradient pattern to the screen.

```cpp
#include <ion/ion.hpp>
#include <SDL.h>

#include <cmath>
#include <cstdint>

#include <iostream>

void render(ion::renderable_window auto & window);
SDL_Color lerp(SDL_Color const & a, SDL_Color const & b, float t);

int main()
{
    // create the sdl event-handler: quit when sdl's quit event is triggered
    ion::event_system events;
    events.subscribe(SDL_QUIT, &ion::input::quit_on_event);

    // initialize sdl - initialize this before other sdl resources
    ion::sdl_context sdl;

    // create a window, specifying the title and dimensions
    ion::render_window window{"A simple window", 800, 600};
    render(window); // render once at the beginning of the program

    // busy loop until the user quits
    while (not ion::input::has_quit()) {
        events.process_queue();
    }
}

// linearly interpolate between two colors
SDL_Color lerp(SDL_Color const & a, SDL_Color const & b, float t)
{
    auto intlerp = [](std::uint8_t x, std::uint8_t y, float t) {
        return static_cast<std::uint8_t>(std::lerp(x, y, t));
    };
    return SDL_Color{intlerp(a.r, b.r, t), intlerp(a.g, b.g, t),
                     intlerp(a.b, b.b, t), 0xff};
}

// draw a fibonacci-like pattern
void render(ion::renderable_window auto & window)
{
    // the initial color
    SDL_Color blue{48, 118, 217, 255};
    // the final color
    SDL_Color red{219, 0, 66, 255};

    // clear the screen
    SDL_SetRenderDrawColor(window, red.r, red.g, red.b, red.a);
    SDL_RenderClear(window);

    // the dimensions of the rect to draw
    SDL_Rect rect{0, 0, 0, 0};
    SDL_GetWindowSize(window, &rect.w, &rect.h);
    rect.w /= 2;

    // draw the fibonacci-like patern
    int n = 8;
    for (int k = 0; k < n; k++) {

        // calculate the intermediate color
        float const t = static_cast<float>(k)/n;
        auto const c = lerp(blue, red, t);

        // draw the rect
        SDL_SetRenderDrawColor(window, c.r, c.g, c.b, c.a);
        SDL_RenderFillRect(window, &rect);

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
    SDL_RenderPresent(window);
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
