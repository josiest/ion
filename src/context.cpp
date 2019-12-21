#include "context.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <sstream>
#include <stdexcept>

ion::context::context(const std::string& title, const ion::rect& dim,
                      unsigned int window_flags, unsigned int render_flags)
{
    // Put the error message in scope so it can be used in clean up
    // outside of the try block.
    std::ostringstream message;

    try {
        // Try to initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            message << "SDL could not initialize! SDL Error: "
                    << SDL_GetError();
            throw std::runtime_error{""};
        }
        // Try to create the window
        window = SDL_CreateWindow(title.c_str(), dim.x, dim.y,
                                  dim.w, dim.h, window_flags);
        if (!window) {
            message << "Window could not be created! SDL Error: "
                    << SDL_GetError();
            throw std::runtime_error{""};
        }
        // Try to create the renderer
        renderer = SDL_CreateRenderer(window, -1, render_flags);
        if (!renderer) {
            message << "Renderer could not be created! SDL Error: "
                    << SDL_GetError();
            throw std::runtime_error{""};
        }
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
        // Try to initialize SDL_image
        if (IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG) < 0) {
            message << "SDL_image couldn't be initialized! SDL_image Error: "
                    << IMG_GetError();
            throw std::runtime_error{""};
        }
    }
    catch (...) {
        // Clean up
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        throw std::runtime_error{message.str()};
    }
}
ion::context::context(const std::string& title,
                      int width, int height,
                      unsigned int window_flags,
                      unsigned int render_flags)

    : context(title, ion::rect{SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, width, height},
              window_flags, render_flags)
{}

ion::context::~context() noexcept
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void ion::context::run()
{
    bool has_quit = false;
    SDL_Event event;
    while (!has_quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                has_quit = true;
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }
}

