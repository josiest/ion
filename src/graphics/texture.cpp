#include "ion/graphics/texture.hpp"
#include <SDL.h>

#include <string>
#include <filesystem>
namespace fs = std::filesystem;

namespace ion {

texture::texture(SDL_Renderer * renderer, SDL_Texture * sdl_texture) noexcept
    : _renderer{renderer}, _texture{sdl_texture}
{
    if (not renderer or not sdl_texture) {
        set_error("texture initialized with null renderer or SDL texture");
    }
}

texture::texture(texture && temp) noexcept
    : _renderer{temp._renderer}, _texture{temp._texture}
{
    temp._renderer = nullptr;
    temp._texture = nullptr;

    set_error(temp.get_error());
    temp.set_error("surface moved to another object");
}

texture::~texture()
{
    // it's possible that the texture may be null if initialization failed
    // so only free the texture if it's not null
    if (_texture) {
        SDL_DestroyTexture(_texture);
    }
    _texture = nullptr;
}

texture
texture::load_image(SDL_Renderer * renderer, fs::path const & filepath)
{
    // load the image and create a texture
    auto image_surface = IMG_Load(filepath.c_str());
    texture image{
        renderer,
        SDL_CreateTextureFromSurface(renderer, image_surface)
    };

    // set the errors respective to what failed first
    if (not image_surface) {
        image.set_error(IMG_GetError());
        return image; // return the failed object
                      // before freeing the null surface
    }
    if (not image) {
        image.set_error(SDL_GetError());
    }
    // clean up and return resulting texture
    SDL_FreeSurface(image_surface);
    return image;
}
}
