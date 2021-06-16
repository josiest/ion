#ifndef ION_GAME_HPP
#define ION_GAME_HPP

#include <SDL2/SDL.h>
#include <vector>
#include <string>

namespace ion {

typedef void (*Listener)();

class app {
    SDL_Window * _window;
    SDL_Renderer * _renderer;
    std::vector<Listener> _update_listeners;
public:
    /**
     * Create an app.
     *
     * :param title: the tile of the app.
     * :param width: the width of the app window.
     * :param height: the height of the app window.
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
