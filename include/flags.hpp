#ifndef ION_FLAGS_HPP
#define ION_FLAGS_HPP

#include <bitset>

namespace ion {
    namespace WindowState {
        /** The possible window flags
         *
         * Fullscreen
         *  The window will be fullscreen.
         *
         * FullscreenDesktop
         *  The window will be fullscreen at the current desktop resolution.
         *
         * OpenGL
         *  OpenGL may be used with the window.
         *
         * Vulkan
         *  Vulkan may be used with the window.
         *
         * Hidden
         *  The window won't be visible.
         *
         * Borderless
         *  No window decoration.
         *
         * Resizable
         *  The window can be resized.
         *
         * Minimized
         *  The window will be minimized
         *
         * Maximized
         *  The window will be maximized
         *
         * InputGrabbed
         *  The window will grab the input focus
         *
         * AllowHighDPI
         *  Window will be in High-DPI mode if suported.
         */
        enum State {
            Fullscreen = 1 << 0,
            FullscreenDesktop = 1 << 1,
            OpenGL = 1 << 2,
            Vulkan = 1 << 3,
            Hidden = 1 << 4,
            Borderless = 1 << 5,
            Resizable = 1 << 6,
            Minimized = 1 << 7,
            Maximized = 1 << 8,
            InputGrabbed = 1 << 9,
            AllowHighDPI = 1 << 10
        };
    }
    using WindowFlags = std::bitset<sizeof(WindowState::State)>;

    /** Get the value of the window flags as an unsigned int */
    namespace WindowState {
        unsigned int to_uint(const WindowFlags&);
    }

    /** The possible Render Flags
     *
     * Software
     *  Renderer is a software fallback.
     *
     * Accelerated
     *  Renderer uses hardware acceleration.
     *
     * PresentVSync
     *  Renderer update is synchronized with the refresh rate.
     *
     * TargetTexture
     *  Renderer supports rendering to texture.
     */
    namespace RenderState {
        enum State {
            Software = 1 << 0,
            Accelerated = 1 << 1,
            PresentVSync = 1 << 2,
            TargetTexture = 1 << 3
        };
    }
    using RenderFlags = std::bitset<sizeof(RenderState::State)>;

    /** Get the value of the rener flags as an unsigned itn */
    namespace RenderState {
        unsigned int to_uint(const RenderFlags&);
    }
}

#endif
