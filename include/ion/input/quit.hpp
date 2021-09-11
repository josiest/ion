#pragma once
#include <SDL2/SDL_events.h>

namespace ion::input {

/**
 * Determine if the user has quit
 */
bool has_quit();

/**
 * Set the quit flag to true
 */
void quit();

/**
 * Convenience method for an event handler
 */
void quit_on_event(SDL_Event const &);
}
