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

SDL_Window * load_window(std::string const & title,
                         std::size_t width, std::size_t height,
                         std::uint32_t flags)
{
    return load_window(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       width, height, flags);
}

SDL_Window * load_window(std::string const & title, int x, int y,
                         std::size_t width, std::size_t height,
                         std::uint32_t flags)
{
    // create the underlying window
    SDL_Window * window = SDL_CreateWindow(
            title.c_str(), x, y,
            static_cast<int>(width), static_cast<int>(height),
            flags);

    // failure if the window couldn't be created
    if (!window) {
        std::stringstream message;
        message << "Window couldn't be created! SDL_Error: "
                << SDL_GetError();
        throw std::runtime_error{message.str()};
    }
    return window;
}

SDL_Renderer * load_renderer(SDL_Window * window,
                             int driver_index, uint32_t flags)
{
    // make sure the given window pointer isn't null
    if (!window) {
        throw std::invalid_argument{"window must not be null!"};
    }

    // create the renderer
    SDL_Renderer * renderer =
        SDL_CreateRenderer(window, driver_index, flags);

    // error if the renderer couldn't be created
    if (!renderer) {
        std::stringstream message;
        message << "Window renderer couldn't be created! SDL_Error: "
                << SDL_GetError();
        SDL_DestroyWindow(window); // clean up
        throw std::runtime_error{message.str()};
    }
    return renderer;
}

blit_window basic_blit_window(std::string const & title,
                              size_t width, size_t height)
{
    return blit_window(load_window(title, width, height));
}

}
