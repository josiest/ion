#include "ion/graphics/font.hpp"

#include <SDL.h>
#include <SDL_ttf.h>

// data types
#include <string>
using namespace std::string_literals;
#include <filesystem>
namespace fs = std::filesystem;
#include <cstdint>
using uint = std::uint32_t;

namespace ion {

font::font(TTF_Font * ttf_font) noexcept
    : _font{ttf_font}
{
    if (not ttf_font) {
        set_error("font initialized as null");
    }
}

font::font(font && temp) noexcept
    : _font(temp._font)
{
    temp._font = nullptr;
    set_error(temp.get_error());
    temp.set_error("font moved to another object");
}

font::~font()
{
    if (_font) {
        TTF_CloseFont(_font);
        _font = nullptr;
    }
}

surface font::soft_render_text(std::string const & text,
                               SDL_Color const & color) noexcept
{
    auto text_surface = TTF_RenderText_Solid(*this, text.c_str(), color);
    surface rendered_text(text_surface);
    if (not rendered_text) {
        rendered_text.set_error(TTF_GetError());
    }
    return rendered_text;
}

font font::from_file(fs::path const & path, uint size) noexcept
{
    std::string message;
    // make sure the font path exists
    if (not fs::exists(path)) {
        message = "The path "s + path.c_str() + " doesn't exist";
    }
    // try to open the font
    auto ttf_font = TTF_OpenFont(path.c_str(), size);
    if (not ttf_font and message.empty()) {
        message = TTF_GetError();
    }
    // create an isotope font and set any error message that occured
    font loaded_font(ttf_font);
    if (not message.empty()) {
        loaded_font.set_error(message);
    }
    return loaded_font;
}

texture
font::render_text(SDL_Renderer * renderer, std::string const & text,
                  SDL_Color const & color) noexcept
{
    std::string message;
    // render the text using this font
    auto text_surface = TTF_RenderText_Solid(*this, text.c_str(), color);
    if (not text_surface) {
        message = TTF_GetError();
    }
    // transform surface into texture
    auto sdl_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

    // update error message only if no other errors have occured yet
    if (not sdl_texture and message.empty()) {
        message = SDL_GetError();
    }
    // only free the text surface if it succesfully rendered
    if (text_surface) {
        SDL_FreeSurface(text_surface);
    }
    // finally, create an isotope texture and set any errors that occured
    texture rendered_text(renderer, sdl_texture);
    if (not message.empty()) {
        rendered_text.set_error(message);
    }
    return rendered_text;
}
}
