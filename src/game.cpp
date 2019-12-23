#include "game.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <sstream>
#include <stdexcept>
#include <filesystem>

ion::game::game(const std::string& title, const ion::rect& dim,
                const std::filesystem::path& project_directory,
                unsigned int window_flags, unsigned int render_flags)

    : project_dir{project_directory},
      resource_dir{project_directory/"resources"},
      textures(resource_dir/"textures")
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
        _window = SDL_CreateWindow(title.c_str(), dim.x, dim.y,
                                  dim.w, dim.h, window_flags);
        if (!_window) {
            message << "Window could not be created! SDL Error: "
                    << SDL_GetError();
            throw std::runtime_error{""};
        }
        // Try to create the renderer
        _renderer = SDL_CreateRenderer(_window, -1, render_flags);
        if (!_renderer) {
            message << "Renderer could not be created! SDL Error: "
                    << SDL_GetError();
            throw std::runtime_error{""};
        }
        SDL_SetRenderDrawColor(_renderer, 0xff, 0xff, 0xff, 0xff);
        // Try to initialize SDL_image
        if (IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG) < 0) {
            message << "SDL_image couldn't be initialized! SDL_image Error: "
                    << IMG_GetError();
            throw std::runtime_error{""};
        }
        // Sort textures by sorting index every time one is constructed
        registry.on_construct<ion::texture>().connect<&ion::sort_textures>();
    }
    catch (...) {
        // Clean up
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
        IMG_Quit();
        SDL_Quit();
        throw std::runtime_error{message.str()};
    }
}
ion::game::game(const std::string& title, int width, int height,
                const std::filesystem::path& project_directory,
                unsigned int window_flags, unsigned int render_flags)

    : game(title, ion::rect{SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, width, height},
              project_directory, window_flags, render_flags)
{}

ion::game::~game() noexcept
{
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    IMG_Quit();
    SDL_Quit();
}

void ion::game::render_update()
{
    SDL_RenderClear(_renderer);

    // Save the default state of the viewport
    SDL_Rect default_viewport;
    SDL_RenderGetViewport(_renderer, &default_viewport);

    auto view = registry.view<ion::texture>();
    for (auto& entity : view) {
        auto& texture = registry.get<ion::texture>(entity);

        // Use custom viewport if defined
        if (auto viewport = registry.try_get<ion::viewport>(entity)) {
            SDL_RenderSetViewport(_renderer, viewport);
        }
        // otherwise use default
        else {
            SDL_RenderSetViewport(_renderer, &default_viewport);
        }
        if (auto sdl_texture = textures.load(_renderer, texture)) {
            SDL_RenderCopy(_renderer, sdl_texture, nullptr, nullptr);
        }
    }
    // Make sure the viewport is back to its default state
    SDL_RenderSetViewport(_renderer, &default_viewport);

    SDL_RenderPresent(_renderer);
}

void ion::game::run()
{
    bool has_quit = false;
    SDL_Event event;
    while (!has_quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                has_quit = true;
            }
        }
        render_update();
    }
}
