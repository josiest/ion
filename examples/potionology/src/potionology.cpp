#include <ion/ion.hpp>
#include <SDL.h>

// datatypes
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include "gui/frame.hpp"

// data structures
#include <vector>

// factories
#include "gui/button_template.hpp"
#include "gui/frame_template.hpp"

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

void render(ion::hardware_renderer & window, frame & main_frame);

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
    auto dejavu_sans = ion::font::from_file(DEJAVUSANS_FILEPATH.c_str(), 30);
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

    // create the main window frame
    SDL_Rect screen_dim{0};
    SDL_GetWindowSize(window, &screen_dim.w, &screen_dim.h);
    frame_template frame_factory(20);
    frame * main_frame = frame_factory.make(screen_dim);

    // create the left and right frames inside the main frame
    auto [left_frame, right_frame] = main_frame->vsplit(frame_factory, .4f);

    // create the factory for making buttons
    button_template button_maker(dejavu_sans, 5);

    // create the buttons for each action
    std::vector<std::string> actions{
        "forage", "brew"
    };
    for (std::string const & action : actions) {
        left_frame->produce_element(&button_maker, window, action);
    }

    // render once at the beginning of the program
    render(window, *main_frame);

    // busy loop until the user quits
    while (not ion::input::has_quit()) {
        events.process_queue();
    }
    return EXIT_SUCCESS;
}

void render(ion::hardware_renderer & window, frame & main_frame)
{
    // clear the screen
    SDL_SetRenderDrawColor(window, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(window);

    main_frame.render(window);
    SDL_RenderPresent(window);
}
