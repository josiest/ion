#pragma once

#include "ion/isotope.hpp"
#include "ion/graphics/surface.hpp"

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

    /** Load a true-type font.
     * \param path the path to the font
     * \param size the font-size to render as
     */
    font(std::filesystem::path const & path, std::uint32_t size) noexcept;
    ~font();

    /** Cast to the underlying TTF_Font */
    inline operator TTF_Font *() noexcept { return _font; }

    /** Render text
     * \param text the text to render
     * \param color the color to render with
     */
    surface render_text(std::string const & text,
                        SDL_Color const & color) noexcept;

private:
    TTF_Font * _font;
};
}
