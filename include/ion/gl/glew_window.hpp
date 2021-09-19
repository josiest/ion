#pragma once

#include "ion/gl/glcontext.hpp"
#include "ion/graphics/window.hpp"
#include "ion/graphics/image.hpp"

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>

#include <stdexcept>
#include <cstdio>

namespace ion {

class glew_window {
public:
    // delete unwanted implicit constructors
    glew_window() = delete;
    glew_window(glew_window const &) = delete;

    /**
     * Create a new glew window
     *
     * \param window the underlying SDL_Window
     * \param SDL_GLContext the underlying SDL_GLContext
     *
     * \throw std::invalid_argument if either window or SDL_GLContext are null
     * \throw std::runtime_error if glew can't initialize
     */
    glew_window(SDL_Window * window, SDL_GLContext glcontext);
    ~glew_window();

    inline operator SDL_Window *() noexcept { return _window; }
    inline SDL_GLContext gl_context() noexcept { return _glcontext; }
private:
    SDL_Window * _window;
    SDL_GLContext _glcontext;
};

glew_window::glew_window(SDL_Window * window, SDL_GLContext glcontext)
    : _window{window}, _glcontext{glcontext}
{
    if (not window) {
        throw std::invalid_argument{"window can't be null!"};
    }
    if (not glcontext) {
        throw std::invalid_argument{"glcontext can't be null!"};
    }
    init_video();

    // try to initialize glew
    glewExperimental = GL_TRUE;
    GLenum const glew_error = glewInit();

    // failure if glew couldn't initialize
    if (glew_error != GLEW_OK) {
        std::string const message = "Couldn't initialize GLEW! glew error: ";
        std::string const error =
            reinterpret_cast<char const *>(glewGetErrorString(glew_error));
        throw std::runtime_error{message + error};
    }

    // try to use vsync
    if (SDL_GL_SetSwapInterval(1) < 0) {
        // warn user if failed to do so
        printf("Couldn't set VSync! SDL Error: %s\n", SDL_GetError());
    }
}

glew_window::~glew_window()
{
    if (_glcontext) {
        SDL_GL_DeleteContext(_glcontext);
    }
    if (_window) {
        SDL_DestroyWindow(_window);
    }
}

glew_window
basic_glew_window(std::string const & title,
                  std::size_t width, std::size_t height,
                  glcontext_args const & attributes = glcontext_args{})
{
    auto window = load_window(title, width, height, SDL_WINDOW_OPENGL);
    auto glcontext = load_glcontext(window, attributes);
    return glew_window(window, glcontext);
}
}
