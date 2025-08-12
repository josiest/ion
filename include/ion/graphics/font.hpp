#pragma once

#include "ion/isotope.hpp"
#include "ion/graphics/surface.hpp"
#include "ion/graphics/texture.hpp"

#include <SDL.h>
#include <SDL_ttf.h>

#include <cstdint>
#include <string>
#include <string_view>
#include <span>
#include <utility>

namespace ion {

/**
 * @brief RAII wrapper around a TTF_Font* with rich helpers.
 *
 * Features:
 *  - Safe move-only ownership of TTF_Font.
 *  - UTF-8 text rendering (solid, blended, shaded, wrapped).
 *  - Hardware (SDL_Texture) and software (SDL_Surface) render paths.
 *  - Metrics/queries: ascent, descent, height, line skip, kerning, style, outline, hinting.
 *  - Measure text (UTF-8).
 *  - Glyph availability/metrics helpers (32-bit codepoints).
 *  - Load from file, file (DPI), and memory (RWops).
 *
 * Errors are stored in the base isotope via set_error().
 */
class font : public isotope {
public:
    // non-owning constructors are not allowed
    font() = delete;
    font(const font&) = delete;

    /// Take ownership of an existing TTF_Font* (may be null).
    explicit font(TTF_Font* ttf_font) noexcept
    : _font(ttf_font) {}

    /// Move constructor.
    font(font&& temp) noexcept
    : isotope(std::move(temp)), _font(std::exchange(temp._font, nullptr)) {}

    /// Move assignment.
    font& operator=(font&& temp) noexcept {
        if (this != &temp) {
            if (_font) {
                TTF_CloseFont(_font);
            }
            // move base error state as well
            isotope::operator=(std::move(temp));
            _font = std::exchange(temp._font, nullptr);
        }
        return *this;
    }

    /// Destructor closes the font if owned.
    ~font() {
        if (_font) {
            TTF_CloseFont(_font);
            _font = nullptr;
        }
    }

    // ---------------------------------------------------------------------
    // Accessors
    // ---------------------------------------------------------------------

    /// Cast to underlying TTF_Font* (mutable).
    inline operator TTF_Font*() noexcept { return _font; }
    /// Cast to underlying TTF_Font* (const).
    inline operator const TTF_Font*() const noexcept { return _font; }

    /// Direct accessors.
    [[nodiscard]] inline TTF_Font*       get()       noexcept { return _font; }
    [[nodiscard]] inline const TTF_Font* get() const noexcept { return _font; }
    [[nodiscard]] inline bool valid() const noexcept           { return _font != nullptr; }

    // ---------------------------------------------------------------------
    // Rendering – Software (SDL_Surface wrapped by ion::surface)
    // ---------------------------------------------------------------------

    /**
     * @brief Render UTF-8 text (antialiased) to a software surface.
     */
    [[nodiscard]] surface soft_render_text(std::string_view text,
                                           SDL_Color const& color) const noexcept
    {
        std::string message;
        SDL_Surface* s = TTF_RenderUTF8_Blended(_font, std::string(text).c_str(), color);
        if (!s) message = TTF_GetError();

        surface out(s);
        if (!message.empty()) out.set_error(message);
        return out;
    }

    /**
     * @brief Render UTF-8 text (solid) to a software surface.
     * Faster, non-antialiased.
     */
    [[nodiscard]] surface soft_render_text_solid(std::string_view text,
                                                 SDL_Color const& color) const noexcept
    {
        std::string message;
        SDL_Surface* s = TTF_RenderUTF8_Solid(_font, std::string(text).c_str(), color);
        if (!s) message = TTF_GetError();

        surface out(s);
        if (!message.empty()) out.set_error(message);
        return out;
    }

    /**
     * @brief Render UTF-8 text (shaded) to a software surface with background color.
     */
    [[nodiscard]] surface soft_render_text_shaded(std::string_view text,
                                                  SDL_Color const& fg,
                                                  SDL_Color const& bg) const noexcept
    {
        std::string message;
        SDL_Surface* s = TTF_RenderUTF8_Shaded(_font, std::string(text).c_str(), fg, bg);
        if (!s) message = TTF_GetError();

        surface out(s);
        if (!message.empty()) out.set_error(message);
        return out;
    }

    /**
     * @brief Render UTF-8 wrapped text (antialiased) to a software surface.
     * @param wrap_width Wrap width in pixels (0 means no wrapping).
     */
    [[nodiscard]] surface soft_render_text_wrapped(std::string_view text,
                                                   SDL_Color const& color,
                                                   std::uint32_t wrap_width) const noexcept
    {
        std::string message;
        SDL_Surface* s = TTF_RenderUTF8_Blended_Wrapped(_font, std::string(text).c_str(), color, wrap_width);
        if (!s) message = TTF_GetError();

        surface out(s);
        if (!message.empty()) out.set_error(message);
        return out;
    }

    // ---------------------------------------------------------------------
    // Rendering – Hardware (SDL_Texture wrapped by ion::texture)
    // ---------------------------------------------------------------------

    /**
     * @brief Render UTF-8 text (antialiased) into a hardware texture for the given renderer.
     */
    template <typename Renderer>
    [[nodiscard]] texture render_text(Renderer& renderer,
                                      std::string_view text,
                                      SDL_Color const& color) const noexcept
    {
        std::string message;

        SDL_Surface* text_surface = TTF_RenderUTF8_Blended(_font, std::string(text).c_str(), color);
        if (!text_surface) {
            message = TTF_GetError();
        }

        SDL_Texture* sdl_texture = nullptr;
        if (text_surface) {
            sdl_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
            if (!sdl_texture && message.empty()) {
                message = SDL_GetError();
            }
            SDL_FreeSurface(text_surface);
        }

        texture out(renderer, sdl_texture);
        if (!message.empty()) out.set_error(message);
        return out;
    }

    /**
     * @brief Render UTF-8 text (solid) into a hardware texture.
     */
    template <typename Renderer>
    [[nodiscard]] texture render_text_solid(Renderer& renderer,
                                            std::string_view text,
                                            SDL_Color const& color) const noexcept
    {
        std::string message;

        SDL_Surface* text_surface = TTF_RenderUTF8_Solid(_font, std::string(text).c_str(), color);
        if (!text_surface) {
            message = TTF_GetError();
        }

        SDL_Texture* sdl_texture = nullptr;
        if (text_surface) {
            sdl_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
            if (!sdl_texture && message.empty()) {
                message = SDL_GetError();
            }
            SDL_FreeSurface(text_surface);
        }

        texture out(renderer, sdl_texture);
        if (!message.empty()) out.set_error(message);
        return out;
    }

    /**
     * @brief Render UTF-8 text (shaded) into a hardware texture.
     */
    template <typename Renderer>
    [[nodiscard]] texture render_text_shaded(Renderer& renderer,
                                             std::string_view text,
                                             SDL_Color const& fg,
                                             SDL_Color const& bg) const noexcept
    {
        std::string message;

        SDL_Surface* text_surface = TTF_RenderUTF8_Shaded(_font, std::string(text).c_str(), fg, bg);
        if (!text_surface) {
            message = TTF_GetError();
        }

        SDL_Texture* sdl_texture = nullptr;
        if (text_surface) {
            sdl_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
            if (!sdl_texture && message.empty()) {
                message = SDL_GetError();
            }
            SDL_FreeSurface(text_surface);
        }

        texture out(renderer, sdl_texture);
        if (!message.empty()) out.set_error(message);
        return out;
    }

    /**
     * @brief Render UTF-8 wrapped text (antialiased) into a hardware texture.
     * @param wrap_width Wrap width in pixels (0 means no wrapping).
     */
    template <typename Renderer>
    [[nodiscard]] texture render_text_wrapped(Renderer& renderer,
                                              std::string_view text,
                                              SDL_Color const& color,
                                              std::uint32_t wrap_width) const noexcept
    {
        std::string message;

        SDL_Surface* text_surface =
            TTF_RenderUTF8_Blended_Wrapped(_font, std::string(text).c_str(), color, wrap_width);
        if (!text_surface) {
            message = TTF_GetError();
        }

        SDL_Texture* sdl_texture = nullptr;
        if (text_surface) {
            sdl_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
            if (!sdl_texture && message.empty()) {
                message = SDL_GetError();
            }
            SDL_FreeSurface(text_surface);
        }

        texture out(renderer, sdl_texture);
        if (!message.empty()) out.set_error(message);
        return out;
    }

    // ---------------------------------------------------------------------
    // Metrics & Queries
    // ---------------------------------------------------------------------

    [[nodiscard]] inline int ascent()    const noexcept { return _font ? TTF_FontAscent(_font)     : 0; }
    [[nodiscard]] inline int descent()   const noexcept { return _font ? TTF_FontDescent(_font)    : 0; }
    [[nodiscard]] inline int height()    const noexcept { return _font ? TTF_FontHeight(_font)     : 0; }
    [[nodiscard]] inline int line_skip() const noexcept { return _font ? TTF_FontLineSkip(_font)   : 0; }

    [[nodiscard]] inline int get_style()  const noexcept { return _font ? TTF_GetFontStyle(_font)      : 0; }
    inline void set_style(int style) noexcept { if (_font) TTF_SetFontStyle(_font, style); }

    [[nodiscard]] inline int  get_outline() const noexcept { return _font ? TTF_GetFontOutline(_font) : 0; }
    inline void set_outline(int px) noexcept { if (_font) TTF_SetFontOutline(_font, px); }

    [[nodiscard]] inline int get_hinting() const noexcept { return _font ? TTF_GetFontHinting(_font)   : 0; }
    inline void set_hinting(int hinting) noexcept { if (_font) TTF_SetFontHinting(_font, hinting); }

    [[nodiscard]] inline bool get_kerning() const noexcept { return _font ? TTF_GetFontKerning(_font) != 0 : false; }
    inline void set_kerning(bool on) noexcept { if (_font) TTF_SetFontKerning(_font, on ? 1 : 0); }

    /**
     * @brief Measure an UTF-8 string (width,height).
     */
    [[nodiscard]] std::pair<int,int> measure_utf8(std::string_view text) const noexcept {
        int w = 0, h = 0;
        if (_font && TTF_SizeUTF8(_font, std::string(text).c_str(), &w, &h) != 0) {
            // Store error but still return (0,0) on failure.
            const char* err = TTF_GetError();
            if (err && *err) const_cast<font*>(this)->set_error(err);
            w = 0; h = 0;
        }
        return {w, h};
    }

    /**
     * @brief Check if a glyph is provided by the font (32-bit codepoint).
     */
    [[nodiscard]] bool glyph_is_provided(char32_t codepoint) const noexcept {
        if (!_font) return false;
        return TTF_GlyphIsProvided32(_font, static_cast<Uint32>(codepoint)) != 0;
    }

    /**
     * @brief Get glyph metrics for a 32-bit codepoint. Returns false on failure.
     */
    [[nodiscard]] bool glyph_metrics(char32_t codepoint,
                                     int& minx, int& maxx,
                                     int& miny, int& maxy,
                                     int& advance) const noexcept
    {
        if (!_font) return false;
        return TTF_GlyphMetrics32(_font, static_cast<Uint32>(codepoint),
                                  &minx, &maxx, &miny, &maxy, &advance) == 0;
    }

    // ---------------------------------------------------------------------
    // Loading
    // ---------------------------------------------------------------------

    /**
     * @brief Load a font from file at the given point size.
     */
    [[nodiscard]] static font from_file(std::string_view path, std::uint16_t size) noexcept {
        std::string message;
        TTF_Font* f = TTF_OpenFont(std::string(path).c_str(), static_cast<int>(size));
        if (!f) {
            message = TTF_GetError();
        }
        font out(f);
        if (!message.empty()) out.set_error(message);
        return out;
    }

    /**
     * @brief Load a font from file at the given point size and DPI.
     *        (Requires SDL_ttf >= 2.0.18)
     */
    [[nodiscard]] static font from_file_dpi(std::string_view path, std::uint16_t size,
                                            std::uint32_t hdpi, std::uint32_t vdpi) noexcept
    {
#if SDL_TTF_VERSION_ATLEAST(2,0,18)
        std::string message;
        TTF_Font* f = TTF_OpenFontDPI(std::string(path).c_str(), static_cast<int>(size),
                                      static_cast<unsigned>(hdpi), static_cast<unsigned>(vdpi));
        if (!f) message = TTF_GetError();
        font out(f);
        if (!message.empty()) out.set_error(message);
        return out;
#else
        font out(nullptr);
        out.set_error("TTF_OpenFontDPI requires SDL_ttf >= 2.0.18");
        return out;
#endif
    }

    /**
     * @brief Load a font from memory at the given point size. Keeps RWops open.
     */
    [[nodiscard]] static font from_memory(std::span<const std::byte> bytes, std::uint16_t size) noexcept {
        std::string message;

        auto* rw = SDL_RWFromConstMem(bytes.data(), static_cast<int>(bytes.size()));
        if (!rw) {
            message = SDL_GetError();
            font out(nullptr);
            out.set_error(message);
            return out;
        }

        // TTF_OpenFontRW takes ownership when freesrc != 0
        TTF_Font* f = TTF_OpenFontRW(rw, 1, static_cast<int>(size));
        if (!f) {
            message = TTF_GetError();
        }
        font out(f);
        if (!message.empty()) out.set_error(message);
        return out;
    }

private:
    TTF_Font* _font { nullptr };
};

} // namespace ion
