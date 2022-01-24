
// frameworks
#include <SDL.h>
#include <ion/ion.hpp>
#include <gold/gold.hpp>

// datatypes
#include <cmath>
#include <cstdint>
#include <cstdlib>

// data structures and resource handlers
#include <vector>
#include <memory>

// i/o
#include <filesystem>
#include <iostream>

// aliases
namespace fs = std::filesystem;
using uint = std::uint32_t;

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

    // load fonts and color definitions
    auto colors = au::load_colors(config_dir/"colors.yaml");
    if (not colors) {
        std::cout << "Unable to load colors! " << colors.error() << std::endl;
        return EXIT_FAILURE;
    }

    auto fonts = au::load_all_fonts(font_dir);
    if (not fonts) {
        std::cout << "Unable to load fonts! " << fonts.error() << std::endl;
        return EXIT_FAILURE;
    }

    // create the factory that will make buttons
    au::button_factory::update_fonts(au::observe_fonts(*fonts));
    au::button_factory::update_colors(*colors);
    auto button_maker = au::button_factory::from_file(config_dir/"button.yaml");
    if (not button_maker) {
        std::cout << "Unable to load button configuration! "
                  << button_maker.error() << std::endl;
        return EXIT_FAILURE;
    }

    // create a frame to render buttons in
    SDL_Rect action_bounds{0, 0, 3*screen_width/7, screen_height};
    auto action_frame = au::frame::from_file(
            window, action_bounds, config_dir/"frame.yaml");
    if (not action_frame) {
        std::cout << "Unable to load frame configuration! "
                  << action_frame.error() << std::endl;
        return EXIT_FAILURE;
    }

    // create a button
    auto forage_button = action_frame->produce_text_widget(
            *button_maker, "forage");
    if (not forage_button) {
        std::cout << "Unable to create a forage button! "
                  << forage_button.error() << std::endl;
        return EXIT_FAILURE;
    }

    while (not ion::input::has_quit()) {
        events.process_queue();

        // clear the screen white
        SDL_SetRenderDrawColor(window, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(window);

        // draw all the widgets in the frame
        action_frame->render();
        SDL_RenderPresent(window);
    }
    return EXIT_SUCCESS;
}
