#include <ion/ion.hpp>
#include <SDL.h>

// datatypes
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include "div.hpp"

// factories
#include "button_template.hpp"

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

void render(ion::hardware_renderer & window, divider & main_div,
            ion::texture & text);

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
    auto window = ion::hardware_renderer::basic_window("Potionology", 800, 600);
    if (not window) {
        std::cout << window.get_error() << std::endl;
        return EXIT_FAILURE;
    }
    // create the font
    auto dejavu_sans = ion::font::from_file(DEJAVUSANS_FILEPATH.c_str(), 40);
    if (not dejavu_sans) {
        std::cout << "Unable to create font! " + dejavu_sans.get_error();
        return EXIT_FAILURE;
    }

    // render the button text
    SDL_Color const black{0, 0, 0, 255};
    auto button_text = dejavu_sans.render_text(window, "Click me!", black);
    if (not button_text) {
        std::cout << "Unable to render button! " + button_text.get_error();
        return EXIT_FAILURE;
    }

    // create the main window div
    SDL_Rect screen_dim{0};
    SDL_GetWindowSize(window, &screen_dim.w, &screen_dim.h);
    divider main_div(screen_dim);

    // create the factory for making buttons
    button_template button_maker(40);
    main_div.produce_element_from(&button_maker);

    render(window, main_div, button_text); // render once at the beginning of the program

    // busy loop until the user quits
    while (not ion::input::has_quit()) {
        events.process_queue();
    }
    return EXIT_SUCCESS;
}

void render(ion::hardware_renderer & window, divider & main_div,
            ion::texture & text)
{
    // clear the screen
    SDL_SetRenderDrawColor(window, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(window);

    main_div.render(window);

    // get the button dimensions
    int button_width, button_height;
    SDL_QueryTexture(text, nullptr, nullptr, &button_width, &button_height);

    // get the screen dimensions
    int screen_width, screen_height;
    SDL_GetWindowSize(window, &screen_width, &screen_height);

    // calculate the button position
    SDL_Rect button_rect{
        (screen_width - button_width)/2, (screen_height - button_height)/2,
        button_width, button_height
    };

    SDL_SetRenderDrawColor(window, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderFillRect(window, &button_rect);

    // render the text to the button
    SDL_RenderCopy(window, text, nullptr, &button_rect);

    SDL_RenderPresent(window);
}
