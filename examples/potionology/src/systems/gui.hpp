#pragma once

// frameworks
#include <SDL.h>
#include <ion/events.hpp>
#include <gold/gold.hpp>

// callbacks

/** Flip activation between two menus. */
au::click_fn flip_menus(au::frame & a, au::frame & b);
