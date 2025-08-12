#pragma once

#include "ion/isotope.hpp"
#include "ion/graphics/surface.hpp"
#include "ion/graphics/texture.hpp"

#include <SDL.h>
#include <SDL_ttf.h>

#include <string>
#include <cstdint>

namespace ion {

/**
 * A font resource handler
 */
class font : public isotope {
public:
    // delete unwanted implicit constructors
    font() = delete;
    font(font const &) = delete;
    font(TTF_Font * ttf_font) noexcept;
    font(font && temp) noexcept;
    ~font();

    /** Cast to the underlying TTF_Font */
    inline operator TTF_Font *() noexcept { return _font; }

    /** Render text using a software renderer.
     * \param text the text to render
     * \param color the color to render with
     */
    surface soft_render_text(std::string const & text,
                             SDL_Color const & color) noexcept;

    /** Render text using a hardware renderer.
     * \param renderer the renderer to use
     * \param text the text to render
     * \param color the color to render with
     */
    texture render_text(renderer_resource auto & renderer,
                        std::string const & text,
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

    /** Load a true-type font.
     * \param path the path to the font
     * \param size the font-size to render as
     */
    static font from_file(const std::string_view & path, std::uint16_t size) noexcept;
private:
    TTF_Font * _font;
};
}
