#pragma once

#include "ion/isotope.hpp"
#include "ion/graphics/surface.hpp"
#include "ion/graphics/texture.hpp"

#include <SDL.h>
#include <SDL_ttf.h>

#include <filesystem>
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
    texture render_text(SDL_Renderer * renderer,
                        std::string const & text,
                        SDL_Color const & color) noexcept;

    /** Load a true-type font.
     * \param path the path to the font
     * \param size the font-size to render as
     */
    static font from_file(std::filesystem::path const & path,
                          std::uint32_t size) noexcept;
private:
    TTF_Font * _font;
};
}
