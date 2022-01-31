#include "systems/gui.hpp"

// frameworks
#include <SDL.h>
#include <gold/gold.hpp>

au::click_fn flip_menus(au::frame & a, au::frame & b)
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
