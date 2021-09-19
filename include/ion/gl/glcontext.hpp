#pragma once

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_video.h>

#include <unordered_map>

#include <stdexcept>
#include <string>

namespace ion {

using glcontext_args = std::unordered_map<SDL_GLattr, int>;

/**
 * Create a new SDL_GLContext
 *
 * \param window the SDL_Window to associate with the context
 * \param attributes the SDL_GLContext attributes
 *
 * \return a new SDL_GLContext associated with window
 *
 * \throw std::invalid_argument if the context couldn't be created
 */
SDL_GLContext load_glcontext(SDL_Window * window,
                             glcontext_args const & attributes = glcontext_args{})
{
    // set all the specified opengl attributes
    for (auto [attr, value] : attributes) {
        SDL_GL_SetAttribute(attr, value);
    }
    // create the opengl context
    auto glcontext = SDL_GL_CreateContext(window);

    // error if context couldn't be created
    if (not glcontext) {
        std::string message = "GL context couldn't be created! SDL_Error: ";
        std::string error = SDL_GetError();
        throw std::runtime_error{message + error};
    }

    return glcontext;
}
}
