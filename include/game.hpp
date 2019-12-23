#ifndef ION_GAME_HPP
#define ION_GAME_HPP

#include "texture.hpp"
#include <SDL2/SDL.h>

#include <entt/entt.hpp>
#include <cstdint>
#include <string>
#include <filesystem>

namespace ion {
    using rect = SDL_Rect;

    class game {
        SDL_Window * _window;
        SDL_Renderer * _renderer;

        /** Render all sprites */
        void render_update();
    public:
        /** Path to the poject directory */
        const std::filesystem::path project_dir;

        /** Path to resource directory */
        const std::filesystem::path resource_dir;

        /** Entity-Component registry */
        entt::registry registry;

        /** Texture bank */
        texture_bank textures;

        /** Create a new game.
         *
         * This game will have a window with the given title, with dimensions
         * width x height, and with its upper-left corner at (x, y).
         *
         * Window and Renderer Flags may be specified for initializing the
         * window, and a project directory may be specified. The project
         * directory defaults to the current working directory.
         */
        game(const std::string&, const rect&,
             const std::filesystem::path& = std::filesystem::current_path(),
             unsigned int = SDL_WINDOW_SHOWN,
             unsigned int = SDL_RENDERER_ACCELERATED);

        /** Create a new game.
         *
         * This game will have a window with the given title, with
         * dimensions width x height and will be created in the middle of the
         * screen.
         *
         * Window and renderer Flags may be specified for initializing the
         * window and a project directory may be specified. The project
         * directory defaults to the current working directory.
         */
        game(const std::string&, int, int,
             const std::filesystem::path& = std::filesystem::current_path(),
             unsigned int = SDL_WINDOW_SHOWN,
             unsigned int = SDL_RENDERER_ACCELERATED);

        /** Destroy this game */
        ~game();

        /** The game's renderer */
        inline SDL_Renderer * renderer() {return _renderer;}

        /** The main loop of this game */
        void run();
    };
}

#endif
