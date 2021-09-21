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
    {SDL_GL_CONTEXT_MAJOR_VERSION, 3}, {SDL_GL_CONTEXT_MINOR_VERSION, 1},
    {SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE}
};

/** A type alias for a SDL_GLattr-int pair */
using attr_pair = std::pair<SDL_GLattr, int>;

/** Set a GL attribute */
inline void set_glattr(attr_pair const & item)
{ SDL_GL_SetAttribute(item.first, item.second); }

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
    inline glwindow(SDL_Window * window, SDL_GLContext glcontext);

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
     * GL attributes default to use the 3.1 core profile
     */
    inline glwindow(std::string const & title,
                    std::uint32_t width, std::uint32_t height,
                    std::uint32_t flags=0,
                    glcontext_args const & attributes=default_gl_attributes);

    /**
     * Create a glew window
     *
     * \param title the title of the window
     *
     * \param x the x-coordinate of the window
     * \param y the y-coordinate of the windwo
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
     * GL attributes default to use the 3.1 core profile
     */
    inline glwindow(std::string const & title, int x, int y,
                    std::uint32_t width, std::uint32_t height,
                    std::uint32_t flags=0,
                    glcontext_args const & attributes=default_gl_attributes);

    inline ~glwindow();

    /** Determine if the window initialized properly */
    inline bool good() const noexcept
    { return _window != nullptr and _glcontext != nullptr; }

    /** Determine if the window enountered an unrecoverable error */
    inline bool bad() const noexcept
    { return nullptr == _window or nullptr == _glcontext; }

    /** Determine if vsync was set */
    inline bool is_vsynced() const noexcept
    { return good() and _error.empty(); }

    /** The error message explaining why initialization failed */
    inline std::string error() const noexcept { return _error; }

    /** The underlying SDL_Window */
    inline SDL_Window * sdl_window() noexcept { return _window; }

    /** The underyling SDL_GLContxt */
    inline SDL_GLContext gl_context() noexcept { return _glcontext; }
private:
    SDL_Window * _window;
    SDL_GLContext _glcontext;
    std::string _error;

    void _init_glew() noexcept;
};

glwindow::glwindow(SDL_Window * window, SDL_GLContext glcontext)
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
                   std::uint32_t flags, glcontext_args const & attributes)

    : glwindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
               width, height, flags, attributes)
{}

glwindow::glwindow(std::string const & title, int x, int y,
                   std::uint32_t width, std::uint32_t height,
                   std::uint32_t flags, glcontext_args const & attributes)
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
