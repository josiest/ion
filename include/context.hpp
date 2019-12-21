#ifndef ION_CONTEXT_HPP
#define ION_CONTEXT_HPP

#include <SDL2/SDL.h>

#include <entt/entt.hpp>
#include <cstdint>

#include <string>

namespace ion {
    using rect = SDL_Rect;

    class context {
        SDL_Window * window;
        SDL_Renderer * renderer;
    public:
        /** Entity-Component registry */
        entt::registry registry;

        /** Create a new context.
         *
         * This context will have a window with the given title, with dimensions
         * width x height, and with its upper-left corner at (x, y). Window and
         * Renderer Flags may be specified for initializing the window.
         */
        context(const std::string&, const rect&,
                unsigned int = SDL_WINDOW_SHOWN,
                unsigned int = SDL_RENDERER_ACCELERATED);

        /** Create a new context.
         *
         * This context will have a window with the given title, with
         * dimensions width x height and will be created in the middle of the
         * screen. Window and renderer Flags may be specified for initializing
         * the window
         */
        context(const std::string&, int, int,
                unsigned int = SDL_WINDOW_SHOWN,
                unsigned int = SDL_RENDERER_ACCELERATED);

        /** Destroy this context */
        ~context();

        /** The main loop of this context */
        void run();
    };
}

#endif
