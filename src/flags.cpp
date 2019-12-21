#include "flags.hpp"
#include <SDL2/SDL.h>

unsigned int ion::WindowState::to_uint(const ion::WindowFlags& flags)
{
    using namespace ion::WindowState;
    unsigned int _flags = 0;
    if (flags[Fullscreen]) {
        _flags |= SDL_WINDOW_FULLSCREEN;
    }
    if (flags[FullscreenDesktop]) {
        _flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    if (flags[OpenGL]) {
        _flags |= SDL_WINDOW_OPENGL;
    }
    if (flags[Vulkan]) {
        _flags |= SDL_WINDOW_VULKAN;
    }
    if (flags[Hidden]) {
        _flags |= SDL_WINDOW_HIDDEN;
    }
    if (flags[Borderless]) {
        _flags |= SDL_WINDOW_BORDERLESS;
    }
    if (flags[Resizable]) {
        _flags |= SDL_WINDOW_RESIZABLE;
    }
    if (flags[Minimized]) {
        _flags |= SDL_WINDOW_MINIMIZED;
    }
    if (flags[Maximized]) {
        _flags |= SDL_WINDOW_MAXIMIZED;
    }
    if (flags[InputGrabbed]) {
        _flags |= SDL_WINDOW_INPUT_GRABBED;
    }
    if (flags[AllowHighDPI]) {
        _flags |= SDL_WINDOW_ALLOW_HIGHDPI;
    }
    return _flags;
}

unsigned int ion::RenderState::to_uint(const RenderFlags& flags)
{
    using namespace ion::RenderState;
    unsigned int _flags = 0;
    if (flags[Software]) {
        _flags |= SDL_RENDERER_SOFTWARE;
    }
    if (flags[Accelerated]) {
        _flags |= SDL_RENDERER_ACCELERATED;
    }
    if (flags[PresentVSync]) {
        _flags |= SDL_RENDERER_PRESENTVSYNC;
    }
    if (flags[TargetTexture]) {
        _flags |= SDL_RENDERER_TARGETTEXTURE;
    }
    return _flags;
}
