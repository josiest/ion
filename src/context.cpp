#include "context.hpp"
#include "flags.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <sstream>
#include <stdexcept>

ion::context::context(const std::string& title,
                      int x, int y, int width, int height,
                      const ion::window_flags& wflags,
                      const ion::render_flags& rflags)
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
        auto wflags_ = ion::window_state::to_uint(wflags);
        window = SDL_CreateWindow(title.c_str(), x, y,
                                  width, height, wflags_);
        if (!window) {
            message << "Window could not be created! SDL Error: "
                    << SDL_GetError();
            throw std::runtime_error{""};
        }
        // Try to create the renderer
        auto rflags_ = ion::render_state::to_uint(rflags);
        renderer = SDL_CreateRenderer(window, -1, rflags_);
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
                      const ion::window_flags& wflags,
                      const ion::render_flags& rflags)
    : context(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
              width, height, wflags, rflags)
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

