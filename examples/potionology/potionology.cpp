#include <ion/ion.hpp>
#include <SDL.h>

// datatypes
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <filesystem>

// i/o
#include <iostream>
#include <memory>

// aliases
namespace fs = std::filesystem;
using uint = std::uint32_t;

// path constants
fs::path const ASSET_DIR = fs::absolute("../assets");
fs::path const FONT_DIR = ASSET_DIR/"fonts";
fs::path const DEJAVUSANS_FILEPATH = FONT_DIR/"DejaVuSans.ttf";

void render(ion::window_resource auto & window, ion::surface & text)
{

    // clear the screen and update
    auto screen = SDL_GetWindowSurface(window);
    SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0, 0, 0));

    // draw the button
    int constexpr width = 300;
    int constexpr height = 100;
    SDL_Rect button{
        (screen->w - width)/2, (screen->h - height)/2, width, height
    };
    SDL_FillRect(screen, &button, SDL_MapRGB(screen->format, 255, 255, 255));
    SDL_BlitSurface(text, nullptr, screen, &button);
    SDL_UpdateWindowSurface(window);
}

int main()
{
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
    auto window = ion::software_renderer::basic_window("Potionology", 800, 600);
    if (not window) {
        std::cout << window.get_error() << std::endl;
        return EXIT_FAILURE;
    }
    // create the static font
    ion::font dejavu_sans{DEJAVUSANS_FILEPATH.c_str(), 40};
    if (not dejavu_sans) {
        window.set_error("Unable to create font! " + dejavu_sans.get_error());
        return EXIT_FAILURE;
    }

    // render the button text
    SDL_Color const black{0, 0, 0, 255};
    auto button_text = dejavu_sans.render_text("Click me!", black);
    if (not button_text) {
        window.set_error("Unable to render button! " + button_text.get_error());
        return EXIT_FAILURE;
    }
    render(window, button_text); // render once at the beginning of the program

    // busy loop until the user quits
    while (not ion::input::has_quit()) {
        events.process_queue();
    }
    return EXIT_SUCCESS;
}
