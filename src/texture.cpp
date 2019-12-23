#include "texture.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <stdexcept>
#include <sstream>

//ion::texture& ion::texture::operator=(texture&& t)
//{
//    return ion::texture{std::move(t.relative_path)};
//}

SDL_Texture * ion::texture_bank::load(SDL_Renderer * renderer,
                                      const std::string& relative_path)
{
    // Make sure the renderer is valid
    if (!renderer) {
        throw std::invalid_argument{"Renderer must not be null!"};
    }
    auto path = texture_path/relative_path; // The path to the texture

    // If the texture has already been loaded, just return the shared texture
    auto search = textures.find(path.c_str());
    if (search != textures.end()) {
        return search->second.get();
    }

    // Try to load the image as an SDL_Surface
    auto surface = IMG_Load(path.c_str());
    if (!surface) {
        std::ostringstream message{"Unable to load image "};
        message << path << "! SDL_Image Error: " << IMG_GetError();
        throw std::runtime_error{message.str()};
    }

    // Try to convert the surface into an SDL_Texture
    auto texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::ostringstream message{"Unable to create texture from "};
        message << path << "! SDL Error: " << SDL_GetError();
        throw std::runtime_error{message.str()};
    }

    // Insert the texture into the bank and return the shared texture
    {
        ion::unique_texture texture_ptr(texture, &SDL_DestroyTexture);
        textures.insert(std::make_pair(path, std::move(texture_ptr)));
    }
    return texture;
}
SDL_Texture * ion::texture_bank::load(SDL_Renderer * renderer,
                                      const texture& texture) {
    return load(renderer, texture.relative_path);
}
