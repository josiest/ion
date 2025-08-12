#include "ion/graphics/font.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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

font font::from_file(const std::string_view & path, std::uint16_t size) noexcept
{
    std::string message;
    // make sure the font path exists
    if (not fs::exists(path)) {
        message = "The path "s + path.begin() + " doesn't exist";
    }
    // try to open the font
    auto * ttf_font = TTF_OpenFont(path.begin(), size);
    if (not ttf_font and message.empty()) {
        message = TTF_GetError();
    }
    // create an isotope font and set any error message that occurred
    font loaded_font(ttf_font);
    if (not message.empty()) {
        loaded_font.set_error(message);
    }
    return loaded_font;
}

}
