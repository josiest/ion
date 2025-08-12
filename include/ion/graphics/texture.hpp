#pragma once

#include "ion/isotope.hpp"
#include "ion/window/resource.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string_view>

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
    static texture load_image(renderer_resource auto & renderer,
                              const std::string_view & path)
    {
        // load the image and create a texture
        auto * image_surface = IMG_Load(path.begin());
        texture image{
            renderer,
            SDL_CreateTextureFromSurface(renderer, image_surface)
        };
    
        // set the errors respective to what failed first
        if (not image_surface) {
            image.set_error(IMG_GetError());
            return image; // return the failed object before freeing the null surface
        }
        if (not image) {
            image.set_error(IMG_GetError());
        }
        // clean up and return resulting texture
        SDL_FreeSurface(image_surface);
        return image;
    }
private:
    SDL_Renderer * _renderer;
    SDL_Texture * _texture;
};
}
