#pragma once

#include <SDL_video.h>
#include <SDL_opengl.h>

#include <cstdint>
#include <string>

#include <unordered_map>
#include <utility>
#include <algorithm>

namespace ion {

/** A type alias for a map of SDL_GLattrs to their values */
using glcontext_args = std::unordered_map<SDL_GLattr, int>;

/** The default GL context attributes to use */
inline glcontext_args const default_gl_attributes{
    {SDL_GL_CONTEXT_MAJOR_VERSION, 3}, {SDL_GL_CONTEXT_MINOR_VERSION, 3},
    {SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE}
};

/** A type alias for a SDL_GLattr-int pair */
using attr_pair = std::pair<SDL_GLattr, int>;

/** Set a GL attribute */
inline void set_glattr(attr_pair const & item)
{
    SDL_GL_SetAttribute(item.first, item.second);
}

/** Determine if the GL attribute is set properly */
inline bool glattr_is_set(attr_pair const & item) {
    int value;
    SDL_GL_GetAttribute(item.first, &value);
    return value == item.second;
}

class glwindow {
public:
    // delete unwanted implicit constructors
    glwindow() = delete;
    glwindow(glwindow const &) = delete;

    /**
     * Create a glew window with pre-initialized resources
     *
     * \param window the underlying SDL_Window
     * \param SDL_GLContext the underlying SDL_GLContext
     */
    glwindow(SDL_Window * window, SDL_GLContext glcontext) noexcept;

    /**
     * Create a basic glwindow
     *
     * \param title the title of the window
     *
     * \param width the width of the window
     * \param height the height of the window
     *
     * \param flags used to initialize the window
     * \param attributes used to initialize opengl
     *
     * The position of the window is as implemented by SDL_WINDOWPOS_UNDEFINED.
     * Any renderer-specific window flags will be ignored other than
     * SDL_WINDOW_OPENGL, which will be used regardless of it being passed.
     *
     * GL attributes default to use the 3.3 core profile
     */
    glwindow(std::string const & title,
             std::uint32_t width, std::uint32_t height,
             std::uint32_t flags=0,
             glcontext_args const & attributes=default_gl_attributes) noexcept;

    /**
     * Create a glew window
     *
     * \param title the title of the window
     *
     * \param x the x-coordinate of the window
     * \param y the y-coordinate of the window
     *
     * \param width the width of the window
     * \param height the height of the window
     *
     * \param flags used to initialize the window
     * \param attributes used to initialize opengl
     *
     * Any renderer-specific window flags will be ignored other than
     * SDL_WINDOW_OPENGL, which will be used regardless of it being passed.
     *
     * GL attributes default to use the 3.3 core profile
     */
    glwindow(std::string const & title, int x, int y,
             std::uint32_t width, std::uint32_t height,
             std::uint32_t flags=0,
             glcontext_args const & attributes=default_gl_attributes) noexcept;

    ~glwindow();

    /** Determine if the window enountered an unrecoverable error */
    inline bool operator!() const noexcept
    {
        return nullptr == _window or nullptr == _glcontext;
    }

    /** The error message explaining why initialization failed */
    inline std::string const & error() const noexcept { return _error; }

    /** The underlying SDL_Window */
    inline operator SDL_Window *() noexcept { return _window; }

    /** The underyling SDL_GLContxt */
    inline SDL_GLContext gl_context() noexcept { return _glcontext; }
private:
    SDL_Window * _window;
    SDL_GLContext _glcontext;
    std::string _error;
};
}
