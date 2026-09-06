#pragma once
#include "ion/engine/sdl_resources.hpp"

namespace ion
{
class engine;
class IEngineComponent
{
public:
    void owner(engine * new_owner) { _owner = new_owner; }
    engine * owner() const { return _owner; }

    virtual void start() {}
    virtual void quit() {}
    virtual ~IEngineComponent() = default;
private:
    engine * _owner = nullptr;
};

class window_component : public IEngineComponent
{
public:
    window_component(std::string_view title, int width, int height, SDL_WindowFlags flags);
    SDL_Window * get() const;
protected:
    sdl_window window_handle = nullptr;
};

class opengl_component : public IEngineComponent
{
public:
    opengl_component(SDL_Window * window);
    opengl_context gl_context = nullptr;
};

class renderer_component : public IEngineComponent
{
public:
    explicit renderer_component(SDL_Window * window);
    sdl_renderer renderer = nullptr;
};
}
