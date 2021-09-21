#pragma once

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>

#include <cstdint>
#include <string>

#include <unordered_map>
#include <utility>
#include <algorithm>

namespace ion {

/** A type alias for a map of SDL_GLattrs to their values */
using glcontext_args = std::unordered_map<SDL_GLattr, int>;
/** A type alias for a SDL_GLattr-int pair */
using attr_pair = std::pair<SDL_GLattr, int>;

/** Set a GL attribute */
void set_glattr(attr_pair const & item)
{ SDL_GL_SetAttribute(item.first, item.second); }

/** Determine if the GL attribute is set properly */
bool glattr_is_set(attr_pair const & item) {
    int value;
    SDL_GL_GetAttribute(item.first, &value);
    return value == item.second;
}

class glew_window {
public:
    // delete unwanted implicit constructors
    glew_window() = delete;
    glew_window(glew_window const &) = delete;

    /**
     * Create a glew window with pre-initialized resources
     *
     * \param window the underlying SDL_Window
     * \param SDL_GLContext the underlying SDL_GLContext
     */
    glew_window(SDL_Window * window, SDL_GLContext glcontext);
    
    /**
     * Create a basic glew_window
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
     */
    glew_window(std::string const & title,
                std::uint32_t width, std::uint32_t height,
                std::uint32_t flags=0,
                glcontext_args const & attributes=glcontext_args{});

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
     */
    glew_window(std::string const & title, int x, int y,
                std::uint32_t width, std::uint32_t height,
                std::uint32_t flags=0,
                glcontext_args const & attributes=glcontext_args{});

    ~glew_window();

    /** Determine if the window initialized properly */
    inline bool is_ok() const noexcept
    { return _window != nullptr and _glcontext != nullptr; }

    /** Determine if vsync was set */
    inline bool is_vsynced() const noexcept
    { return is_ok() and _error.empty(); }

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

glew_window::glew_window(SDL_Window * window, SDL_GLContext glcontext)
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
    _init_glew();
}

glew_window::glew_window(std::string const & title,
                         std::uint32_t width, std::uint32_t height,
                         std::uint32_t flags, glcontext_args const & attributes)

    : glew_window(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                  width, height, flags, attributes)
{}

glew_window::glew_window(std::string const & title, int x, int y,
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
    _init_glew();
}

void glew_window::_init_glew() noexcept {
    // try to initialize glew
    glewExperimental = GL_TRUE;
    GLenum const glew_error = glewInit();

    // failure if glew couldn't initialize
    if (glew_error != GLEW_OK) {

        // get the error messages
        std::string const message = "Couldn't initialize GLEW! glew error: ";
        std::string const error =
            reinterpret_cast<char const *>(glewGetErrorString(glew_error));

        // clean up
        SDL_DestroyWindow(_window); _window = nullptr;
        SDL_GL_DeleteContext(_glcontext); _glcontext = nullptr;

        _error = message + error;
        return;
    }

    // try to use vsync
    if (SDL_GL_SetSwapInterval(1) < 0) {
        std::string const message = "Couldn't set VSync! SDL_Error: ";
        std::string const error = SDL_GetError();
        _error = message + error;
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
}
