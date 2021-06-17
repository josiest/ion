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
the screen as white.

```cpp
#include "ion.hpp"
#include "SDL2/SDL.h"

// our render function will need acces to the app's renderer
// there are more sophisticated ways of doing this, but for the sake of
// simplicity we'll just use a global variable
SDL_Renderer * RENDERER;

// draw a white screen
void render()
{
    SDL_SetRenderDrawColor(RENDERER, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(RENDERER);
    SDL_RenderPresent(RENDERER);
}

int main(int argc, char * argv[])
{
    // specify the app's title and dimensions
    ion::app simple{"A simple window", 800, 600};

    // load the renderer and render function
    RENDERER = simple.renderer();
    simple.connect_update_listener(&render); 

    // run the app
    simple.run();
    return 0;
}
```
