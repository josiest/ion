// frameworks
#include <SDL.h>
#include <ion/ion.hpp>
#include <gold/gold.hpp>

// datatypes
#include <cmath>
#include <cstdint>
#include <cstdlib> // std::size_t
#include <functional>

// data structures and resource handlers
#include <vector>
#include <memory>

// i/o
#include <filesystem>
#include <iostream>

// systems
#include "systems/gui.hpp"

// aliases
namespace fs = std::filesystem;
using uint = std::uint32_t;

template<class widget_subclass>
au::iwidget * as_widget(widget_subclass * widget) {
    return dynamic_cast<au::iwidget *>(widget);
}

int main()
{
    // path constants
    fs::path const asset_dir = fs::absolute("../assets");
    fs::path const font_dir = asset_dir/"fonts";
    fs::path const config_dir = asset_dir/"config";

    // create the sdl event-handler: quit when sdl's quit event is triggered
    ion::event_system events;
    events.subscribe(SDL_QUIT, &ion::input::quit_on_event);

    // initialize sdl - initialize this before other sdl resources
    ion::sdl_context sdl;
    sdl.init_ttf();
    if (not sdl) {
        std::cout << "error initializing sdl: "
                  << sdl.get_error() << std::endl;
        return EXIT_FAILURE;
    }

    // create a window, specifying the title and dimensions
    uint const screen_width = 800;
    uint const screen_height = 600;
    auto window = ion::hardware_renderer::basic_window(
            "Potionology", screen_width, screen_height);
    if (not window) {
        std::cout << window.get_error() << std::endl;
        return EXIT_FAILURE;
    }

    while (not ion::input::has_quit()) {
        events.process_queue();

        // clear the screen white
        SDL_SetRenderDrawColor(window, 0xff, 0xff, 0xff, 0xff);

        SDL_RenderClear(window);
        SDL_RenderPresent(window);
    }
    return EXIT_SUCCESS;
}
