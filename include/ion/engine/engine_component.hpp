#pragma once
#include "ion/engine/sdl_resources.hpp"

namespace ion
{
class engine;
class IEngineComponent
{
public:
    virtual void set_owner(engine * new_owner);
    virtual engine * get_owner() const;

    virtual void start() {}
    virtual void quit() {}
    virtual ~IEngineComponent();
};

class window_component : public IEngineComponent
{
public:
    sdl_window window;

    void set_owner(engine * new_owner) override;
    engine * get_owner() const override;
protected:
    engine * owner = nullptr;
};

class renderer_component : public IEngineComponent
{
public:
    sdl_renderer renderer;

    void set_owner(engine * new_owner) override;
    engine * get_owner() const override;
protected:
    engine * owner = nullptr;
};
}
