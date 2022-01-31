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

    // create a main menu frame to render buttons in
    fs::path const menu_config = config_dir/"menu.yaml";
    auto main_menu = au::frame::from_file(window, menu_config);
    if (not main_menu) {
        std::cout << "Unable to load frame configuration! "
                  << main_menu.error() << std::endl;
        return EXIT_FAILURE;
    }

    // create the forage button
    auto forage_button = main_menu->produce_text_widget(
            *button_maker, "forage");
    if (not forage_button) {
        std::cout << "Unable to create a forage button! "
                  << forage_button.error() << std::endl;
        return EXIT_FAILURE;
    }
    // save the id of this button when clicked
    events.subscribe_functor(SDL_MOUSEBUTTONDOWN, au::click_button(*forage_button));

    // create the brew button
    auto brew_button = main_menu->produce_text_widget(
            *button_maker, "brew");
    if (not brew_button) {
        std::cout << "Unable to create a brew button! "
                  << brew_button.error() << std::endl;
        return EXIT_FAILURE;
    }
    // save the id of this button when clicked
    events.subscribe_functor(SDL_MOUSEBUTTONDOWN, au::click_button(*brew_button));

    // create the sell button
    auto sell_button = main_menu->produce_text_widget(
            *button_maker, "sell");
    if (not sell_button) {
        std::cout << "Unable to create a sell button! "
                  << sell_button.error() << std::endl;
        return EXIT_FAILURE;
    }
    // save the id of this button when clicked
    events.subscribe_functor(SDL_MOUSEBUTTONDOWN, au::click_button(*sell_button));

    // create the forage menu frame
    auto forage_menu = au::frame::from_file(window, menu_config);
    if (not forage_menu) {
        std::cout << "Unable to load frame configuration from "
                  << menu_config << std::endl
                  << forage_menu.error() << std::endl;
        return EXIT_FAILURE;
    }

    // create the button for foraging in the forest
    auto forest_button = forage_menu->produce_text_widget(
            *button_maker, "forest");
    if (not forest_button) {
        std::cout << "Unable to create a forest button! "
                  << forest_button.error() << std::endl;
        return EXIT_FAILURE;
    }
    // save the id of this button when clicked
    events.subscribe_functor(SDL_MOUSEBUTTONDOWN, au::click_button(*forest_button));

    // create the button for foraging in the field
    auto field_button = forage_menu->produce_text_widget(
            *button_maker, "field");
    if (not field_button) {
        std::cout << "Unable to create a field button! "
                  << field_button.error() << std::endl;
        return EXIT_FAILURE;
    }
    // save the id of this button when clicked
    events.subscribe_functor(SDL_MOUSEBUTTONDOWN, au::click_button(*field_button));

    // create the return button in the forage menu
    auto forage_return = forage_menu->produce_text_widget(
            *button_maker, "return");
    if (not forage_return) {
        std::cout << "Unable to create a forage return button! "
                  << forage_return.error() << std::endl;
        return EXIT_FAILURE;
    }
    // save the id of this button when clicked
    events.subscribe_functor(SDL_MOUSEBUTTONDOWN, au::click_button(*forage_return));

    // the forage menu should be initially deactivated
    forage_menu->deactivate();

    // it's important that these callbacks are registered *after* the
    // au::click_button callbacks have been registered
    auto flip_forage_main = flip_menus(*main_menu, *forage_menu);
    // activate the forage menu when forage is clicked
    events.subscribe_functor(
            SDL_MOUSEBUTTONDOWN,
            au::on_click(au::as_widget(*forage_button), flip_forage_main));
    // deactivate the main menu when forage is clicked
    events.subscribe_functor(
            SDL_MOUSEBUTTONDOWN,
            au::on_click(au::as_widget(*forage_return), flip_forage_main));

    while (not ion::input::has_quit()) {
        events.process_queue();

        // clear the screen white
        SDL_SetRenderDrawColor(window, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(window);

        // draw all the widgets in the frame
        main_menu->render();
        forage_menu->render();
        SDL_RenderPresent(window);
    }
    return EXIT_SUCCESS;
}
