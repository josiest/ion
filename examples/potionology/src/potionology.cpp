
// frameworks
#include <SDL.h>
#include <ion/ion.hpp>
#include <gold/gold.hpp>

// datatypes
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <functional>

// data structures and resource handlers
#include <vector>
#include <memory>

// i/o
#include <filesystem>
#include <iostream>

// aliases
namespace fs = std::filesystem;
using uint = std::uint32_t;

template<class widget_subclass>
au::iwidget * as_widget(widget_subclass * widget) {
    return dynamic_cast<au::iwidget *>(widget);
}

using click_fn = std::function<void(SDL_Point const &, SDL_Rect const &)>;
auto on_click(au::iwidget * button, click_fn callback)
{
    return [button, callback](SDL_Event const & event) {
        // don't do anything if the clicked button isn't active
        if (not button->is_active()) { return; }

        // get the event information
        SDL_Rect const bounds = button->bounds();
        SDL_Point const mouse{event.button.x, event.button.y};

        // call the function
        if (au::within_closed_bounds(mouse, bounds)) {
            callback(mouse, bounds);
        }
    };
}

auto activate_menu(au::frame & menu, bool active)
{
    return [active, &menu](SDL_Point const & mouse, SDL_Rect const & button) {
        if (active) { menu.activate(); }
        else { menu.deactivate(); }
    };
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
    auto main_menu = au::frame::from_file(window, config_dir/"main-menu.yaml");
    if (not main_menu) {
        std::cout << "Unable to load frame configuration! "
                  << main_menu.error() << std::endl;
        return EXIT_FAILURE;
    }
    auto activate_main_menu = activate_menu(*main_menu, true);
    auto deactivate_main_menu = activate_menu(*main_menu, false);

    // create the forage button
    auto forage_button = main_menu->produce_text_widget(
            *button_maker, "forage");
    if (not forage_button) {
        std::cout << "Unable to create a forage button! "
                  << forage_button.error() << std::endl;
        return EXIT_FAILURE;
    }

    // deactivate the main menu when forage is clicked
    events.subscribe_functor(
            SDL_MOUSEBUTTONDOWN,
            on_click(as_widget(*forage_button), deactivate_main_menu));

    while (not ion::input::has_quit()) {
        events.process_queue();

        // clear the screen white
        SDL_SetRenderDrawColor(window, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(window);

        // draw all the widgets in the frame
        main_menu->render();
        SDL_RenderPresent(window);
    }
    return EXIT_SUCCESS;
}
