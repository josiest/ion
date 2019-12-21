#include "context.hpp"
#include "flags.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <sstream>
#include <stdexcept>

ion::Context::Context(const std::string& title,
                      int x, int y, int width, int height,
                      const WindowFlags& window_flags,
                      const RenderFlags& render_flags)
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
        auto window_flags_ = ion::WindowState::to_uint(window_flags);
        window = SDL_CreateWindow(title.c_str(), x, y,
                                  width, height, window_flags_);
        if (!window) {
            message << "Window could not be created! SDL Error: "
                    << SDL_GetError();
            throw std::runtime_error{""};
        }
        // Try to create the renderer
        auto render_flags_ = ion::RenderState::to_uint(render_flags);
        renderer = SDL_CreateRenderer(window, -1, render_flags_);
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
ion::Context::Context(const std::string& title,
                      int width, int height,
                      const ion::WindowFlags& window_flags,
                      const ion::RenderFlags& render_flags)
    : Context(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
              width, height, window_flags, render_flags)
{}

ion::Context::~Context() noexcept
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void ion::Context::run()
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

