#pragma once

// frameworks
#include <SDL.h>
#include <ion/events.hpp>
#include <gold/gold.hpp>

// data types
#include <cstddef>      // std::size_t
#include <functional>   // std::function

// aliases
using click_fn = std::function<void(SDL_MouseButtonEvent const &)>;

// global variables
namespace _g {
extern std::size_t clicked_button;
}

/** Register the button that was clicked. */
void click(au::iwidget * button);
/** Determine if a button was clicked this frame. */
bool is_clicked(au::iwidget * button);

// callbacks

/** Call when creating a button to register when it clicks. */
ion::listener_fn click_button(au::iwidget * button);

/** Call when adding functionality to a button. */
ion::listener_fn on_click(au::iwidget * button, click_fn callback);

/** Flip activation between two menus. */
click_fn flip_menus(au::frame & a, au::frame & b);
