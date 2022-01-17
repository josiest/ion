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

font::font(fs::path const & path, uint size) noexcept
{
    if (not fs::exists(path)) {
        set_error("The path "s + path.c_str() + " doesn't exist");
    }
    _font = TTF_OpenFont(path.c_str(), size);
    if (not _font) {
        set_error(TTF_GetError());
    }
}

font::~font()
{
    if (_font) {
        TTF_CloseFont(_font);
        _font = nullptr;
    }
}

surface font::render_text(std::string const & text,
                          SDL_Color const & color) noexcept
{
    auto text_surface = TTF_RenderText_Solid(*this, text.c_str(), color);
    surface rendered_text(text_surface);
    if (not rendered_text) {
        rendered_text.set_error(TTF_GetError());
    }
    return rendered_text;
}
}
