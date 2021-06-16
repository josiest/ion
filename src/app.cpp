#include "app.hpp"

#include <SDL2/SDL.h>

#include <string>
#include <sstream>
#include <stdexcept>

#include <algorithm>

namespace ion {

app::app(const std::string& title, int width, int height)
{
    // Put the error message in scope so it can be used in clean up
    // outside of the try block.
    std::stringstream message;

    try {
        // Try to initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            message << "SDL could not initialize! SDL Error: "
                    << SDL_GetError();
            throw 1;
        }
        // Try to create the window
        _window = SDL_CreateWindow(
            // title
            title.c_str(),
            // window position
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            // window dimensions
            width, height,
            // additional flags
            SDL_RENDERER_ACCELERATED
        );
        if (!_window) {
            message << "Window could not be created! SDL Error: "
                    << SDL_GetError();
            throw 2;
        }
        // Try to create the renderer
        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
        if (!_renderer) {
            message << "Renderer could not be created! SDL Error: "
                    << SDL_GetError();
            throw 3;
        }
    }
    // Clean up
    catch (int err) {
        switch (err) {
        case 3: // window created but renderer failed
            SDL_DestroyWindow(_window);
        case 2: // sdl initialized but window failed
            SDL_Quit();
        case 1: // sdl failed to initialize
        default:
            break;
        }
        throw std::runtime_error{message.str()};
    }
}
ion::app::~app()
{
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void app::run()
{
    // run until the user quits
    bool has_quit = false;
    while (!has_quit) {

        // poll all events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            // quit if the user wants to
            if (event.type == SDL_QUIT) {
                has_quit = true;
            }
        }

        // update phase
        std::for_each(_update_listeners.begin(), _update_listeners.end(),
                      [](auto callback) { callback(); });
    }
}
}
