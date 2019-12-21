#ifndef ION_CONTEXT_HPP
#define ION_CONTEXT_HPP

#include "flags.hpp"
#include <SDL2/SDL.h>
#include <string>

namespace ion {
    class context {
        SDL_Window * window;
        SDL_Renderer * renderer;

    public:
        /** Create a new context.
         *
         * This context will have a window with the given title, with dimensions
         * width x height, and with its upper-left corner at (x, y). Window and
         * Renderer Flags may be specified for initializing the window.
         */
        context(const std::string&, int, int, int, int,
                const window_flags& = 0, const render_flags& = 0);

        /** Create a new context.
         *
         * This context will have a window with the given title, with
         * dimensions width x height and will be created in the middle of the
         * screen. Window and renderer Flags may be specified for initializing
         * the window
         */
        context(const std::string&, int, int,
                const window_flags& = 0, const render_flags& = 0);

        /** Destroy this context */
        ~context();

        /** The main loop of this context */
        void run();
    };
}

#endif
