#include "systems/gui.hpp"

// frameworks
#include <SDL.h>
#include <ion/events.hpp>
#include <gold/gold.hpp>

namespace _g {
std::size_t clicked_button = 0;
}
void click(au::iwidget * button)
{
    if (not button) {
        _g::clicked_button = 0;
    }
    else {
        _g::clicked_button = button->id();
    }
}
bool is_clicked(au::iwidget * button)
{
    return button->id() == _g::clicked_button;
}

ion::listener_fn click_button(au::iwidget * button)
{
    return [button](SDL_Event const & event) {
        // don't register click if the button isn't active
        if (not button->is_active()) { return; }

        // get the relevant event information
        SDL_Rect const bounds = button->bounds();
        SDL_Point const mouse{event.button.x, event.button.y};

        // register the click if the mouse is in bounds
        if (au::within_closed_bounds(mouse, bounds)) { click(button); }
    };
}

ion::listener_fn on_click(au::iwidget * button, click_fn callback)
{
    return [button, callback](SDL_Event const & event) {
        if (not button->is_active() or not is_clicked(button)) { return; }

        // get the event information
        SDL_Rect const bounds = button->bounds();
        SDL_Point const mouse{event.button.x, event.button.y};

        // call the function
        if (au::within_closed_bounds(mouse, bounds)) { callback(event.button); }
    };
}

click_fn flip_menus(au::frame & a, au::frame & b)
{
    return [&a, &b](SDL_MouseButtonEvent const & event) {
        if (a.is_active()) {
            a.deactivate(); b.activate();
        }
        else {
            a.activate(); b.deactivate();
        }
    };
}
