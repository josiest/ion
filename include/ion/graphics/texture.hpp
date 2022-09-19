#pragma once
#include "ion/isotope.hpp"

#include <SDL.h>
#include <SDL_image.h>

#include <filesystem>
#include <string>

namespace ion {

/**
 * A resource handle for a single texture
 */
class texture : public isotope {
public:
    // delete unwanted implicit constructors
    texture() = delete;
    texture(texture const &) = delete;

    /**
     * Create a texture from an SDL_Texture
     * \param texture the underlying SDL_Texture
     */
    texture(SDL_Renderer * renderer, SDL_Texture * sdl_texture) noexcept;

    /**
     * Create a texture from another temporary texture
     */
    texture(texture && temp) noexcept;
    ~texture();

    /** Cast to the underlying SDL_Texture */
    inline operator SDL_Texture *() noexcept { return _texture; }

    /** Create a texture by loading an image.
     * Note: SDL_Image must be initialized before calling
     */
    static texture
    load_image(SDL_Renderer * renderer, std::filesystem::path const & filepath);
private:
    SDL_Renderer * _renderer;
    SDL_Texture * _texture;
};
}
