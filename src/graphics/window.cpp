// header declarations
#include "ion/graphics/window.hpp"

// types used
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <string>

// functions used
#include "ion/graphics/image.hpp"

// exceptions and messages
#include <stdexcept>
#include <sstream>

namespace ion {

blit_window::blit_window(SDL_Window * window, int img_init_flags)
    : _window{window}
{
    init_video();
    init_image(img_init_flags);
}

blit_window::~blit_window()
{
    if (_window) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
}

render_window::render_window(SDL_Window * window, SDL_Renderer * renderer,
                             int img_init_flags)
    : _window{window}, _renderer{renderer}
{
    init_video();
    init_image(img_init_flags);
}

render_window::~render_window()
{
    // destroy and clear the renderer pointer first
    if (_renderer) {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }

    // destroy and clear the window pointer last
    if (_window) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
}

SDL_Window *
load_window(std::string const & title, size_t width, size_t height,
            int x, int y, uint32_t flags)
{
    // create the underlying window
    SDL_Window * window_ptr = SDL_CreateWindow(
            title.c_str(), x, y,
            static_cast<int>(width), static_cast<int>(height),
            flags);

    // failure if the window couldn't be created
    if (!window_ptr) {
        std::stringstream message;
        message << "Window couldn't be created! SDL_Error: "
                << SDL_GetError();
        throw std::runtime_error{message.str()};
    }
    return window_ptr;
}

SDL_Renderer * load_renderer(SDL_Window * window_ptr,
                             int driver_index, uint32_t flags)
{
    // make sure the given window pointer isn't null
    if (!window_ptr) {
        throw std::invalid_argument{"window_ptr must not be null!"};
    }

    // create the renderer
    SDL_Renderer * renderer =
        SDL_CreateRenderer(window_ptr, driver_index, flags);

    // error if the renderer couldn't be created
    if (!renderer) {
        std::stringstream message;
        message << "Window renderer couldn't be created! SDL_Error: "
                << SDL_GetError();
        SDL_DestroyWindow(window_ptr); // clean up
        throw std::runtime_error{message.str()};
    }
    return renderer;
}

render_window basic_window(std::string const & title,
                           size_t width, size_t height)
{
    // create the window, then create the renderer from the window
    auto window_ptr = load_window(title, width, height);
    return render_window(window_ptr, load_renderer(window_ptr));
}

blit_window basic_blit_window(std::string const & title,
                              size_t width, size_t height)
{
    return blit_window(load_window(title, width, height));
}

}
