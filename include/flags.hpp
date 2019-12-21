#ifndef ION_FLAGS_HPP
#define ION_FLAGS_HPP

#include <bitset>

namespace ion {
    namespace window_state {
        /** The possible window flags
         *
         * fullscreen
         *  The window will be fullscreen.
         *
         * fullscreen_desktop
         *  The window will be fullscreen at the current desktop resolution.
         *
         * opengl
         *  opengl may be used with the window.
         *
         * vulkan
         *  Vulkan may be used with the window.
         *
         * hidden
         *  The window won't be visible.
         *
         * borderless
         *  No window decoration.
         *
         * resizable
         *  The window can be resized.
         *
         * minimized
         *  The window will be minimized
         *
         * maximized
         *  The window will be maximized
         *
         * input_grabbed
         *  The window will grab the input focus
         *
         * high_dpi
         *  Window will be in High-DPI mode if suported.
         */
        enum state {
            fullscreen = 1 << 0,
            fullscreen_desktop = 1 << 1,
            opengl = 1 << 2,
            vulkan = 1 << 3,
            hidden = 1 << 4,
            borderless = 1 << 5,
            resizable = 1 << 6,
            minimized = 1 << 7,
            maximized = 1 << 8,
            input_grabbed = 1 << 9,
            high_dpi = 1 << 10
        };
    }
    using window_flags = std::bitset<sizeof(window_state::state)>;

    /** Get the value of the window flags as an unsigned int */
    namespace window_state {
        unsigned int to_uint(const window_flags&);
    }

    /** The possible render flags
     *
     * software
     *  Renderer is a software fallback.
     *
     * accelerated
     *  Renderer uses hardware acceleration.
     *
     * vsync
     *  Renderer update is synchronized with the refresh rate.
     *
     * target_texture
     *  Renderer supports rendering to texture.
     */
    namespace render_state {
        enum state {
            software = 1 << 0,
            accelerated = 1 << 1,
            vsync = 1 << 2,
            target_texture = 1 << 3
        };
    }
    using render_flags = std::bitset<sizeof(render_state::state)>;

    /** Get the value of the rener flags as an unsigned itn */
    namespace render_state {
        unsigned int to_uint(const render_flags&);
    }
}

#endif
