#ifndef ION_HPP
#define ION_HPP

#include <SDL2/SDL.h>
#include <functional>
#include <vector>
#include <string>

namespace ion {

using Listener = std::function<void()>;

/**
 * An SDL resource/event handler
 */
class app {
    SDL_Window * _window;
    SDL_Renderer * _renderer;
    std::vector<Listener> _update_listeners;
public:
    /**
     * Create an app.
     *
     * :param title: the title of the program.
     * :param width: the width of the window.
     * :param height: the height of the window.
     */
    app(const std::string & title, int width, int height);
    ~app();

    /** The game's window */
    inline SDL_Window * window() { return _window; }
    /** The game's renderer */
    inline SDL_Renderer * renderer() { return _renderer; }

    /** Connect a function to be called on update */
    inline void connect_update_listener(Listener callback)
    { _update_listeners.push_back(callback); }

    /** The main loop of this game */
    void run();
};
}

#endif
