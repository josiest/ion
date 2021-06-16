#ifndef ION_GAME_HPP
#define ION_GAME_HPP

#include <SDL2/SDL.h>
#include <string>

namespace ion {

class app {
    SDL_Window * _window;
    SDL_Renderer * _renderer;
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

    /** The main loop of this game */
    void run();
};
}

#endif
