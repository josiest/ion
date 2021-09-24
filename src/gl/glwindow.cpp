#include <SDL_video.h>
#include <SDL_opengl.h>

#include "ion/gl/glwindow.hpp"

#include <cstdint>
#include <algorithm>

namespace ion {

glwindow::glwindow(SDL_Window * window, SDL_GLContext glcontext) noexcept
    : _window{window}, _glcontext{glcontext}
{
    if (not window and not glcontext) {
        _error = "window and glcontext initialized as null"; return;
    }
    if (not window) {
        _error = "window initialized as null";
        SDL_GL_DeleteContext(_glcontext); _glcontext = nullptr;
        return;
    }
    if (not glcontext) {
        _error = "glcontext initialized as null";
        SDL_DestroyWindow(_window); _window = nullptr;
        return;
    }
}

glwindow::glwindow(std::string const & title,
                   std::uint32_t width, std::uint32_t height,
                   std::uint32_t flags, glcontext_args const & attributes) noexcept

    : glwindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
               width, height, flags, attributes)
{}

glwindow::glwindow(std::string const & title, int x, int y,
                   std::uint32_t width, std::uint32_t height,
                   std::uint32_t flags, glcontext_args const & attributes) noexcept
{
    namespace ranges = std::ranges;

    // include opengl regardless
    flags |= SDL_WINDOW_OPENGL;
    // exclude other renderer-specific flags
    flags &= ~(SDL_WINDOW_VULKAN | SDL_WINDOW_METAL);

    // try to create the window
    _window = SDL_CreateWindow(title.c_str(), x, y, width, height, flags);
    if (not _window) { _error = SDL_GetError(); return; }

    // try to set the gl attributes
    ranges::for_each(attributes, &set_glattr);
    if (not ranges::all_of(attributes, &glattr_is_set)) {
        _error = SDL_GetError();
        SDL_DestroyWindow(_window); _window = nullptr; // clean up
        return;
    }

    // try to create the gl context
    _glcontext = SDL_GL_CreateContext(_window);
    if (not _glcontext) {
        _error = SDL_GetError();
        SDL_DestroyWindow(_window); _window = nullptr; // clean up
        return;
    }
}

glwindow::~glwindow()
{
    if (_glcontext) {
        SDL_GL_DeleteContext(_glcontext);
        _glcontext = nullptr;
    }
    if (_window) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
}
}
