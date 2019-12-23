#ifndef ION_TEXTURE_HPP
#define ION_TEXTURE_HPP

#include <SDL2/SDL.h>
#include <entt/entt.hpp>

#include <string>
#include <memory>
#include <unordered_map>
#include <filesystem>

namespace ion {
    using rect = SDL_Rect;
    struct viewport : rect {
        inline viewport(int x, int y, int w, int h) : rect{x, y, w, h} {}
    };
    struct texture {
        std::string relative_path;
        int sorting_index;

        inline texture(const std::string& path, int i = 0)
            : relative_path{path}, sorting_index{i} {}
    };
    void sort_textures(entt::entity, entt::registry&, texture&);

    using unique_texture =
        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;

    using texture_map =
        std::unordered_map<std::string, unique_texture>;

    class texture_bank {
        texture_map textures;
    public:
        /** The path used to find the texture files */
        const std::filesystem::path texture_path;

        /** Create a new texture bank.
         *
         * The filenames used to load textures to the bank will be relative to
         * texture_path.
         */
        inline texture_bank(const std::string& path) : texture_path{path} {}
        texture_bank() = delete;

        /** Load a new texture from a relative path.
         *
         * Returns a previously loaded texture if it already exists.
         *
         * Throws an error if the texture couldn't be loaded.
         */
        SDL_Texture * load(SDL_Renderer *, const std::string&);
        SDL_Texture * load(SDL_Renderer *, const texture&);
    };
}

#endif
